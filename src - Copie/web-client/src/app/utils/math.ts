export class Vector {
    x: number;
    y: number;

    constructor(x?: number, y?: number) {
        this.x = x ?? 0;
        this.y = y ?? 0;
    }

    set(x: number, y: number) {
        this.x = x;
        this.y = y;
    }

    setEqualTo(v: Vector) {
        this.x = v.x;
        this.y = v.y;
    }
}
