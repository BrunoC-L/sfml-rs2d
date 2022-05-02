import { Component, OnInit } from '@angular/core';

@Component({
    selector: 'app-animated-background',
    templateUrl: './animated-background.component.html',
    styleUrls: ['./animated-background.component.scss'],
})
export class AnimatedBackgroundComponent implements OnInit {
    smallFireOffsets = [-2, 1, 4, 9];

    constructor() {}

    ngOnInit(): void {}
}
