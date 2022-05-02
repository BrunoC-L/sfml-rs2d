import { Injectable } from '@angular/core';
import { Observable, of, Subscriber } from 'rxjs';
import { Texture } from 'src/app/models/Texture';

@Injectable({
    providedIn: 'root',
})
export class AssetsService {
    private textures: Map<string, Texture> = new Map();

    async get(path: string): Promise<Texture> {
        if (this.textures.has(path)) {
            return this.textures.get(path)!;
        } else {
            return this.loadTexture(path);
        }
    }

    private async loadTexture(path: string): Promise<Texture> {
        return new Promise((resolve, reject) => {
            let image = new Image();
            const texture = new Texture(image);
            this.textures.set(path, texture);
            image.onload = () => resolve(texture);
            image.onerror = reject;
            image.src = path;
        });
    }
}
