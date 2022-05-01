export class Texture {
    private _image: HTMLImageElement;

    constructor(image: HTMLImageElement) {
        this._image = image;
    }

    get image() {
        return this._image;
    }
}
