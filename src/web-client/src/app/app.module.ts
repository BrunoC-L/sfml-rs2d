import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { GameViewComponent } from './components/game-view/game-view.component';
import { LoginComponent } from './components/login/login.component';
import { SignUpComponent } from './components/sign-up/sign-up.component';
import { HomescreenComponent } from './components/homescreen/homescreen.component';
import { AnimatedBackgroundComponent } from './components/animated-background/animated-background.component';

@NgModule({
    declarations: [
        AppComponent,
        GameViewComponent,
        LoginComponent,
        SignUpComponent,
        HomescreenComponent,
        AnimatedBackgroundComponent,
    ],
    imports: [BrowserModule, FormsModule, ReactiveFormsModule],
    providers: [],
    bootstrap: [AppComponent],
})
export class AppModule {}
