import { AfterContentChecked, AfterViewInit, Component, ElementRef, HostListener, OnInit, ViewChild } from '@angular/core';
import * as shajs from 'sha.js';

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

  loginData?: { id: number, position: { x: number, y: number } };

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
          this.loginData = msg.data;
          this.drawGameScene();
          break;
      }
    });
  }

  ngAfterViewInit(): void {
    // this.drawGameScene();
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

  @HostListener('window:resize')
  drawGameScene() {
    this.setCanvasSize();
    const canvas = this.gameScene.nativeElement;
    const ctx = canvas.getContext('2d')!;
    ctx.fillStyle = 'black';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    const ground = new Image();


    const cx = Math.floor(this.loginData!.position.x / 64);
    const cy = Math.floor(this.loginData!.position.y / 64);
    const px = this.loginData!.position.x - cx * 64;
    const py = this.loginData!.position.y - cy * 64;

    ground.onload = async function () {
      const img = await createImageBitmap(ground);
      const chunkdx = (px + 0.5) / 64;
      const chunkdy = (py + 0.5) / 64;
      const chunkWidth = 2048;
      ctx.drawImage(img, canvas.width / 2 - chunkWidth * chunkdx, canvas.height / 2 - chunkWidth * chunkdy, chunkWidth, chunkWidth);
    }

    ground.src = `assets/textures-32/chunks-64/${cx}-${cy}-0.png`;


    const player = new Image();
    player.onload = async function () {
      const img = await createImageBitmap(player);
      const tileWidth = 64;
      const tileHeight = 64;
      ctx.drawImage(img, canvas.width / 2 - tileWidth / 2, canvas.height / 2 - tileHeight, tileWidth, tileHeight);
    }

    player.src = `assets/player.png`;
  }


}
