import { Injectable } from '@angular/core';
import { SocketMessageTypes } from 'src/app/enums/SocketMessageTypes';
import { SocketMessageData } from 'src/app/models/SocketMessageData';

@Injectable({
    providedIn: 'root',
})
export class SocketService {
    private readonly CONNECTION_STRING = 'ws://localhost:38839';

    private connection: WebSocket = new WebSocket(this.CONNECTION_STRING);
    private observers: Map<SocketMessageTypes, Set<(data: any) => void>> = new Map();

    constructor() {
        this.connection.addEventListener('message', (event: MessageEvent) => {
            this.onMessageReceived(event);
        });
    }

    send(messageType: SocketMessageTypes, payload: any) {
        this.connection.send(
            JSON.stringify({
                type: messageType,
                data: payload,
            })
        );
    }

    on(type: SocketMessageTypes, callback: (data: any) => void) {
        if (this.observers.has(type)) this.observers.get(type)?.add(callback);
        else this.observers.set(type, new Set([callback]));
    }

    off(type: SocketMessageTypes, callback: (data: any) => void) {
        const targetList = this.observers.get(type);
        targetList?.delete(callback);
        if (targetList?.size === 0) this.observers.delete(type);
    }

    private onMessageReceived(event: MessageEvent) {
        const msg = this.parseMessage(event);
        console.log(msg);
        const concernedObservers = this.observers.get(msg.type);
        concernedObservers?.forEach((callback) => callback(msg.data));
    }

    private parseMessage(event: MessageEvent): SocketMessageData {
        return JSON.parse(event.data);
    }
}
