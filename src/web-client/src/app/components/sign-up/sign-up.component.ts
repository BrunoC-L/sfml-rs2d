import { Component, EventEmitter, Output } from '@angular/core';
import { FormBuilder } from '@angular/forms';
import { AuthService } from 'src/app/services/auth/auth.service';

@Component({
    selector: 'app-sign-up',
    templateUrl: './sign-up.component.html',
    styleUrls: ['./sign-up.component.scss'],
})
export class SignUpComponent {
    @Output() cancel = new EventEmitter();

    signUpForm = this.formBuilder.group({
        username: 'simon',
        password: 'simon',
    });

    constructor(private formBuilder: FormBuilder, private authService: AuthService) {}

    trySignUp(): void {
        const { username, password } = this.signUpForm.value;
        this.authService.signUp(username, password);
    }
}
