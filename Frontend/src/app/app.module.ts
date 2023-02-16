import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LeftsideComponent } from './leftside/leftside.component';
import { RightsideComponent } from './rightside/rightside.component';
import { HttpClientModule } from '@angular/common/http'; //HttpClientmodule
import { RestService } from './rest.service'; //RestService to use HTTP



@NgModule({
  declarations: [
    AppComponent,
    LeftsideComponent,
    RightsideComponent,
    
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    HttpClientModule //HttpClientmodule
  ],
  providers: [RestService], //RestService to use HTTP
  bootstrap: [AppComponent]
})
export class AppModule { }
