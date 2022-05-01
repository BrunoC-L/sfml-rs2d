import { Component } from '@angular/core';
import { HomescreenNavigation } from 'src/app/enums/HomescreenNavigation';

@Component({
    selector: 'app-homescreen',
    templateUrl: './homescreen.component.html',
    styleUrls: ['./homescreen.component.scss'],
})
export class HomescreenComponent {
    NavOptions = HomescreenNavigation;
    currentNavigation: HomescreenNavigation = HomescreenNavigation.DEFAULT;

    constructor() {}

    showLoginForm(): void {
        this.currentNavigation = HomescreenNavigation.LOGIN;
    }

    showSignUpForm(): void {
        this.currentNavigation = HomescreenNavigation.SIGN_UP;
    }

    returnToHomeScreen(): void {
        this.currentNavigation = HomescreenNavigation.DEFAULT;
    }
}
