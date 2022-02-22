import { Injectable } from '@angular/core';
import { BehaviorSubject, Observable } from 'rxjs';
import * as shajs from 'sha.js';
import { SocketMessageTypes } from 'src/app/enums/SocketMessageTypes';
import { MessageLogInReceived } from 'src/app/models/socketMessages/MessageLogInReceived';
import { MessageLogInToSend } from 'src/app/models/socketMessages/MessageLogInToSend';
import { MessageSaltsReceived } from 'src/app/models/socketMessages/MessageSaltsReceived';
import { MessageSaltsRequestToSend } from 'src/app/models/socketMessages/MessageSaltsRequestToSend';
import { MessageSignUpToSend } from 'src/app/models/socketMessages/MessageSignUpToSend';
import { SocketService } from '../socket/socket.service';

@Injectable({
    providedIn: 'root',
})
export class AuthService {
    isLoggedIn = false;

    private _loginConfirmed: BehaviorSubject<MessageLogInReceived | undefined>;

    private loginPassword?: string;
    private loginUsername?: string;

    constructor(private socketService: SocketService) {
        socketService.on(SocketMessageTypes.SALTS, (data) =>
            this.handleSaltsReceived(data)
        );
        socketService.on(SocketMessageTypes.LOGIN, (data) =>
            this.handleLoginReceived(data)
        );
        this._loginConfirmed = new BehaviorSubject<MessageLogInReceived | undefined>(
            undefined
        );
    }

    public get loginConfirmed(): Observable<MessageLogInReceived | undefined> {
        return this._loginConfirmed.asObservable();
    }

    signUp(username: string, password: string) {
        const payload: MessageSignUpToSend = {
            username: username,
            passwordHash: new shajs.sha256().update(password).digest('hex'),
        };
        this.socketService.send(SocketMessageTypes.SIGN_UP, payload);
    }

    login(username: string, password: string) {
        this.loginPassword = password;
        this.loginUsername = username;
        const payload: MessageSaltsRequestToSend = {
            username: username,
        };
        this.socketService.send(SocketMessageTypes.SALTS_REQUEST, payload);
    }

    private doLogin(saltsData: MessageSaltsReceived) {
        const hash1 = new shajs.sha256().update(this.loginPassword ?? '').digest('hex');
        const hash2 = new shajs.sha256().update(saltsData.permsalt + hash1).digest('hex');
        const hash3 = new shajs.sha256().update(saltsData.tempsalt + hash2).digest('hex');

        const payload: MessageLogInToSend = {
            username: this.loginUsername ?? '',
            passwordHash: hash3,
        };
        this.socketService.send(SocketMessageTypes.LOGIN, payload);
    }

    private handleSaltsReceived(salts: MessageSaltsReceived): void {
        this.doLogin(salts);
    }

    private handleLoginReceived(playerData: MessageLogInReceived): void {
        this.isLoggedIn = true;
        this._loginConfirmed.next(playerData);
    }
}
