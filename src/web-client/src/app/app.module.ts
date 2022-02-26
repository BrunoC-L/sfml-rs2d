import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { GameViewComponent } from './components/game-view/game-view.component';
import { LoginComponent } from './components/login/login.component';

@NgModule({
    declarations: [AppComponent, GameViewComponent, LoginComponent],
    imports: [BrowserModule, FormsModule],
    providers: [],
    bootstrap: [AppComponent],
})
export class AppModule {}
