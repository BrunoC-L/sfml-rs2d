import { AfterContentChecked, AfterViewInit, Component, ElementRef, HostListener, OnInit, ViewChild } from '@angular/core';
import * as shajs from 'sha.js';

const PIXELS_PER_TILE = 32;
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements AfterViewInit {

  @ViewChild('gameScene', { static: false }) gameScene!: ElementRef<HTMLCanvasElement>;

  loggedIn = false;
  username = 'simon';
  password = 'simon';
  permSalt = undefined;
  tickTime = 0;
  prevPlayerX = 0;
  prevPlayerY = 0;
  targetX = 0;
  targetY = 0;
  playerX = 0;
  playerY = 0;
  prevCx = 0;
prevCy = 0;

  ground?: HTMLImageElement;
  player?: HTMLImageElement;

  connection: WebSocket;

  constructor() {
    this.connection = new WebSocket("ws://localhost:38839");
    this.connection.addEventListener('message', (ev: MessageEvent) => {
      const msg = this.parseMessage(ev);
      switch (msg.type) {
        case 'salts':
          this.doLogin(msg.data);
          break;
        case 'login':
          this.loggedIn = true;
          this.playerX = msg.data.position.x;
          this.playerY = msg.data.position.y;
          this.targetX = this.playerX;
          this.targetY = this.playerY;
          this.prevPlayerX = this.playerX;
          this.prevPlayerY = this.playerY;
          this.doGameLoop();
          break;
        case 'positions':
          this.updatePlayerPos(msg.data);
          break;
      }
    });
  }

  ngAfterViewInit(): void {
    this.setupGameScene();
    this.gameScene.nativeElement.addEventListener('click', (event) => { this.handleMouseClick(event) });
  }

  signUp() {
    this.connection.send(JSON.stringify({
      type: "sign up",
      data: {
        username: this.username,
        passwordHash: new shajs.sha256().update(this.password).digest('hex'),
      }
    }))
  }

  login() {
    this.connection.send(JSON.stringify({
      type: "salts request",
      data: {
        username: this.username,
      }
    }));
  }

  parseMessage(event: MessageEvent): any {
    return JSON.parse(event.data.split('|')[0]);
  }

  doLogin(data: any) {
    const hash1 = new shajs.sha256().update(this.password).digest('hex');
    const hash2 = new shajs.sha256().update(data.permsalt + hash1).digest('hex');
    const hash3 = new shajs.sha256().update(data.tempsalt + hash2).digest('hex');
    this.connection.send(JSON.stringify({
      type: "login",
      data: {
        username: this.username,
        passwordHash: hash3,
      }
    }));

  }

  setCanvasSize() {
    const canvas = this.gameScene.nativeElement;
    canvas.width = window.innerWidth - 200;
    canvas.height = window.innerHeight - 200;
  }

  doGameLoop() {
    requestAnimationFrame(() => { this.doGameLoop() });
    if (!this.loggedIn) return;
    this.update();
    this.drawEverything();
  }

  update() {
    if (!this.loggedIn) return;
    const current = (new Date()).getTime();
    const tickRateMS = 60;
    const delta = (current - this.tickTime) / tickRateMS;
    this.playerX = this.clamp(this.prevPlayerX + (this.targetX - this.prevPlayerX) * delta, this.prevPlayerX, this.targetX);
    this.playerY = this.clamp(this.prevPlayerY + (this.targetY - this.prevPlayerY) * delta, this.prevPlayerY, this.targetY);
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
    const cx = Math.floor(this.playerX / 64);
    const cy = Math.floor(this.playerY / 64);
    const px = this.playerX - cx * 64;
    const py = this.playerY - cy * 64;
    if (!this.ground || cx !== this.prevCx || cy !== this.prevCy) {
      const ground = new Image();
      ground.src = `assets/textures-32/chunks-64/${cx}-${cy}-0.png`;
      this.prevCx = cx;
      this.prevCy = cy;
      this.ground = ground;
    } else {
      this.drawGround(this.ground, px, py);
    }
  }

  async drawGround(ground: HTMLImageElement, px: number, py: number) {
    if(ground.loading) return;
    const canvas = this.gameScene.nativeElement;
    const ctx = canvas.getContext('2d')!;
    const img = await createImageBitmap(ground);
    const chunkdx = (px + 0.5) / 64;
    const chunkdy = (py + 0.5) / 64;
    const chunkWidth = 2048;
    ctx.drawImage(img, canvas.width / 2 - chunkWidth * chunkdx, canvas.height / 2 - chunkWidth * chunkdy, chunkWidth, chunkWidth);
  }

  updatePlayerImage() {
    if(!this.player) {
    const player = new Image();
    player.src = `assets/player.png`;
    this.player = player;
    } else {
      this.drawPlayer(this.player);
    }
  }

  async drawPlayer(player: any) {
    if(player.loading) return;
    const canvas = this.gameScene.nativeElement;
    const ctx = canvas.getContext('2d')!;
    const img = await createImageBitmap(player);
    const playerWidth = 64;
    const playerHeight = 64;
    ctx.drawImage(img, canvas.width / 2 - playerWidth / 2, canvas.height / 2 - playerHeight * 3 / 4, playerWidth, playerHeight);
  }

  handleMouseClick(event: any) {
    if(!this.loggedIn) return;
    const canvas = this.gameScene.nativeElement;
    this.connection.send(JSON.stringify({
      type: "walk",
      data: {
        x: this.playerX + (event.layerX - canvas.width / 2) / PIXELS_PER_TILE,
        y: this.playerY + (event.layerY - canvas.height / 2) / PIXELS_PER_TILE,
      }
    }))
    event.preventDefault();
  }

  updatePlayerPos(data: any) {
    this.tickTime = (new Date()).getTime();
    this.prevPlayerX = this.playerX;
    this.prevPlayerY = this.playerY;
    this.targetX = data[0].x;
    this.targetY = data[0].y;
  }
}
