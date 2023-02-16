import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http'; //importing the HTTPClient module
import { Observable } from 'rxjs';//to be able to use Observables
import { Message } from './Message.model';
import { HttpHeaders } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})


export class RestService {

  FlaskServiceURL : string = "http://127.0.0.1:5000/"; //the URL where the Flask RestService runs
  Payloadstr : string =""; 
  Payloadnmbr : number = 0; 
  

  constructor(private http: HttpClient) { }

  Open_Close(message: Message) : Observable<Message>{ //to send an opening or closing signal
    const httpheader = new HttpHeaders({//making a header to indicate the JSON format__ authorization maybe not important
      'Content-Type':'application/json',
  
    });

    return this.http.post<Message>(this.FlaskServiceURL, JSON.stringify(message), {headers: httpheader,responseType:"text" as "json"});
  }

  
  refreshTime(message: Message){
    const httpheader = new HttpHeaders({
      'Content-Type':'application/json'
    })

    return this.http.post<Message>(this.FlaskServiceURL, JSON.stringify(message), {headers: httpheader,responseType:"text" as "json"});
  }


  TestServiceGET() : Observable<string>{ //test it with requesting a string
    return this.http.get(this.FlaskServiceURL, {responseType: 'text'});
  }

  
}
