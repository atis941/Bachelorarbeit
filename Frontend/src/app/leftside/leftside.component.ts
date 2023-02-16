import { Component, OnInit } from '@angular/core';
import { Message } from '../Message.model';
import { RestService } from '../rest.service'; //The rest service to be able to use HTTP

@Component({
  selector: 'app-leftside',
  templateUrl: './leftside.component.html',
  styleUrls: ['./leftside.component.css']
})
export class LeftsideComponent implements OnInit {

  constructor(private REST : RestService) { }

  BlindOpen() { //function to open the window
    this.REST.Open_Close(new Message("windowBlind/open", "ON"))
            .subscribe();
  }

  BlindClose() { //function to close the window
    this.REST.Open_Close(new Message("windowBlind/close", "ON"))
            .subscribe();
  }

  ngOnInit(): void { // use it to write out if the connection to the broker has sucessfully made

  }

}
