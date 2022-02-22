import {
    AfterViewInit,
    Component,
    ElementRef,
    HostListener,
    ViewChild,
} from '@angular/core';
import * as shajs from 'sha.js';
import { SocketMessageTypes } from './enums/SocketMessageTypes';
import { SocketService } from './services/socket/socket.service';
import { Vector } from './utils/math';

const PIXELS_PER_TILE = 32;
@Component({
    selector: 'app-root',
    templateUrl: './app.component.html',
    styleUrls: ['./app.component.scss'],
})
export class AppComponent implements AfterViewInit {
    @ViewChild('gameScene', { static: false }) gameScene!: ElementRef<HTMLCanvasElement>;

    loggedIn = false;
    username = 'simon';
    password = 'simon';
    permSalt = undefined;
    tickTime = 0;
    prevPlayerPos = new Vector();
    playerPos = new Vector();
    target = new Vector();
    prevChunkPos = new Vector();

    ground?: HTMLImageElement;
    player?: HTMLImageElement;

    constructor(private socketService: SocketService) {
        socketService.on(SocketMessageTypes.SALTS, (data) =>
            this.handleSaltsReceived(data)
        );
        socketService.on(SocketMessageTypes.LOGIN, (data) =>
            this.handleLoginReceived(data)
        );
        socketService.on(SocketMessageTypes.POSITIONS, (data) =>
            this.handlePositionsReceived(data)
        );
    }

    handleSaltsReceived(salts: any): void {
        this.doLogin(salts);
    }

    handleLoginReceived(playerData: any): void {
        this.loggedIn = true;
        this.playerPos.set(playerData.position.x, playerData.position.y);
        this.target.set(this.playerPos.x, this.playerPos.y);
        this.prevPlayerPos.set(this.playerPos.x, this.playerPos.y);
        this.doGameLoop();
    }

    handlePositionsReceived(positions: any): void {
        this.updatePlayerPos(positions);
    }

    ngAfterViewInit(): void {
        this.setupGameScene();
        this.gameScene.nativeElement.addEventListener('click', (event) => {
            this.handleMouseClick(event);
        });
    }

    signUp() {
        const payload = {
            username: this.username,
            passwordHash: new shajs.sha256().update(this.password).digest('hex'),
        };
        this.socketService.send(SocketMessageTypes.SIGN_UP, payload);
    }

    login() {
        const payload = {
            username: this.username,
        };
        this.socketService.send(SocketMessageTypes.SALTS_REQUEST, payload);
    }

    parseMessage(event: MessageEvent): any {
        return JSON.parse(event.data.split('|')[0]);
    }

    doLogin(data: any) {
        const hash1 = new shajs.sha256().update(this.password).digest('hex');
        const hash2 = new shajs.sha256().update(data.permsalt + hash1).digest('hex');
        const hash3 = new shajs.sha256().update(data.tempsalt + hash2).digest('hex');

        const payload = {
            username: this.username,
            passwordHash: hash3,
        };
        this.socketService.send(SocketMessageTypes.LOGIN, payload);
    }

    setCanvasSize() {
        const canvas = this.gameScene.nativeElement;
        canvas.width = window.innerWidth - 200;
        canvas.height = window.innerHeight - 200;
    }

    doGameLoop() {
        requestAnimationFrame(() => {
            this.doGameLoop();
        });
        if (!this.loggedIn) return;
        this.update();
        this.drawEverything();
    }

    update() {
        if (!this.loggedIn) return;
        const current = new Date().getTime();
        const tickRateMS = 60;
        const delta = (current - this.tickTime) / tickRateMS;
        this.playerPos.x = this.clamp(
            this.prevPlayerPos.x + (this.target.x - this.prevPlayerPos.x) * delta,
            this.prevPlayerPos.x,
            this.target.x
        );
        this.playerPos.y = this.clamp(
            this.prevPlayerPos.y + (this.target.y - this.prevPlayerPos.y) * delta,
            this.prevPlayerPos.y,
            this.target.y
        );
    }

    clamp(val: number, l: number, h: number): number {
        if (l > h) {
            const temp = l;
            l = h;
            h = temp;
        }
        if (val < l) return l;
        else if (val > h) return h;
        return val;
    }

    @HostListener('window:resize')
    drawEverything() {
        if (!this.loggedIn) return;
        this.setupGameScene();
        this.updateGroundImage();
        this.updatePlayerImage();
    }

    setupGameScene() {
        this.setCanvasSize();
        const canvas = this.gameScene.nativeElement;
        const ctx = canvas.getContext('2d')!;
        ctx.fillStyle = 'black';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
    }

    updateGroundImage() {
        const cx = Math.floor(this.playerPos.x / 64);
        const cy = Math.floor(this.playerPos.y / 64);
        if (!this.ground || cx !== this.prevChunkPos.x || cy !== this.prevChunkPos.y) {
            const ground = new Image();
            ground.src = `assets/textures-32/chunks-64/${cx}-${cy}-0.png`;
            this.prevChunkPos.x = cx;
            this.prevChunkPos.y = cy;
            this.ground = ground;
        } else {
            const px = this.playerPos.x - cx * 64;
            const py = this.playerPos.y - cy * 64;
            this.drawGround(this.ground, px, py);
        }
    }

    async drawGround(ground: HTMLImageElement, px: number, py: number) {
        if (ground.loading) return;
        const canvas = this.gameScene.nativeElement;
        const ctx = canvas.getContext('2d')!;
        const img = await createImageBitmap(ground);
        const chunkdx = (px + 0.5) / 64;
        const chunkdy = (py + 0.5) / 64;
        const chunkWidth = 2048;
        ctx.drawImage(
            img,
            canvas.width / 2 - chunkWidth * chunkdx,
            canvas.height / 2 - chunkWidth * chunkdy,
            chunkWidth,
            chunkWidth
        );
    }

    updatePlayerImage() {
        if (!this.player) {
            const player = new Image();
            player.src = `assets/player.png`;
            this.player = player;
        } else {
            this.drawPlayer(this.player);
        }
    }

    async drawPlayer(player: any) {
        if (player.loading) return;
        const canvas = this.gameScene.nativeElement;
        const ctx = canvas.getContext('2d')!;
        const img = await createImageBitmap(player);
        const playerWidth = 64;
        const playerHeight = 64;
        ctx.drawImage(
            img,
            canvas.width / 2 - playerWidth / 2,
            canvas.height / 2 - (playerHeight * 3) / 4,
            playerWidth,
            playerHeight
        );
    }

    handleMouseClick(event: any) {
        if (!this.loggedIn) return;
        const canvas = this.gameScene.nativeElement;
        const payload = {
            x: this.playerPos.x + (event.layerX - canvas.width / 2) / PIXELS_PER_TILE,
            y: this.playerPos.y + (event.layerY - canvas.height / 2) / PIXELS_PER_TILE,
        };
        this.socketService.send(SocketMessageTypes.WALK, payload);
        event.preventDefault();
    }

    updatePlayerPos(data: any) {
        this.tickTime = new Date().getTime();
        this.prevPlayerPos.setEqualTo(this.playerPos);
        this.target.setEqualTo(data[0]);
    }
}
