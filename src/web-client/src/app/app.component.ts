import { Component, OnInit } from '@angular/core';
import * as shajs from 'sha.js';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  username = 'simon';
  password = 'simon';
  permSalt = undefined;

  connection: WebSocket;

  constructor() {
      this.connection = new WebSocket("ws://localhost:38839");
      this.connection.addEventListener('message', (ev:MessageEvent)=>{
        const msg = this.parseMessage(ev);
        switch(msg.type) {
          case 'salts':
            this.doLogin(msg.data);
            break;
          case 'login':
            console.log("BOIIIIIIIIIIIIIIIII");
            break;
        }
      });
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

  parseMessage(event: MessageEvent):any {
    return JSON.parse(event.data.split('|')[0]);
  }

  doLogin(data: any) {

    const hash1 =  new shajs.sha256().update(this.password).digest('hex');

    const hash2 =  new shajs.sha256().update(data.permsalt + hash1).digest('hex');
    const hash3 =  new shajs.sha256().update(data.tempsalt + hash2).digest('hex');
    this.connection.send(JSON.stringify({
      type: "login",
      data: {
        username: this.username,
         passwordHash: hash3,
      }
    }));

  }


}
