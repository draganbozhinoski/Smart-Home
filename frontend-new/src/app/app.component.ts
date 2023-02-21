import { Component, OnInit } from '@angular/core';
import { Injectable } from '@angular/core';
import { MqttClientService, IMqttMessage, MqttService } from 'ngx-mqtt';
import { Subscription, BehaviorSubject,switchMap } from 'rxjs';
import {HttpClient} from '@angular/common/http'
import { formatDate } from '@angular/common';
import {InputSwitchModule} from 'primeng/inputswitch';
import { PrimeNGConfig } from 'primeng/api';
import { MenuItem } from 'primeng/api'
import { Window } from 'src/Window';
import { Pm } from 'src/Pm';

@Injectable({
  providedIn: 'root'
})
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit{
  messages: string[] = [];

  constructor() {
  }

  ngOnInit(): void {

  }
}