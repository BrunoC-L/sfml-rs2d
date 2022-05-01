import { Component, EventEmitter, Output } from '@angular/core';
import { FormBuilder } from '@angular/forms';
import { Observable } from 'rxjs';
import { AuthService } from 'src/app/services/auth/auth.service';

@Component({
    selector: 'app-login',
    templateUrl: './login.component.html',
    styleUrls: ['./login.component.scss'],
})
export class LoginComponent {
    @Output() cancel = new EventEmitter();

    loginForm = this.formBuilder.group({
        username: 'simon',
        password: 'simon',
    });

    constructor(private formBuilder: FormBuilder, private authService: AuthService) {}

    tryLogin(): void {
        const { username, password } = this.loginForm.value;
        this.authService.login(username, password);
    }
}
