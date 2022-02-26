import { SocketMessageTypes } from '../enums/SocketMessageTypes';

export type SocketMessageData = {
    type: SocketMessageTypes;
    data: any;
};
