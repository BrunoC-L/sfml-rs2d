import { CHUNKS_FOLDER, TEXTURES_FOLDER } from 'src/app/utils/constants';

const ASSETS_FOLDER = 'assets';

export class TexturesPaths {
    static readonly PLAYER = `${ASSETS_FOLDER}/player.png`;

    static getChunkPath(chunkX: number, chunkY: number): string {
        const chunkFolder = `${ASSETS_FOLDER}/${TEXTURES_FOLDER}/${CHUNKS_FOLDER}`;
        return `${chunkFolder}/${chunkX}-${chunkY}-0.png`;
    }
}
