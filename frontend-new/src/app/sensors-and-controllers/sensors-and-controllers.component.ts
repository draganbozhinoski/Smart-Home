import { formatDate } from '@angular/common';
import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { IMqttMessage, MqttService } from 'ngx-mqtt';
import { PrimeNGConfig } from 'primeng/api';
import { Subscription } from 'rxjs';
import { Window } from 'src/Window';

@Component({
  selector: 'app-sensors-and-controllers',
  templateUrl: './sensors-and-controllers.component.html',
  styleUrls: ['./sensors-and-controllers.component.css']
})
export class SensorsAndControllersComponent implements OnInit{
  windowSubscription: Subscription | undefined;
  lightSubscription: Subscription | undefined;
  
  klimaOn: Boolean = false;
  windowOpen: Boolean = false;
  lastWindowUpdate: string = "unknown";
  auto: Boolean = false;

  movementDetected: Boolean = false;
  lightOn: Boolean = false;
  lastLightUpdate: string = "unknown";

  constructor(private _mqttService: MqttService,private http:HttpClient, private primengConfig: PrimeNGConfig) {
    this.primengConfig.ripple = true;
  }
  ngOnInit(): void {
    this.http.get<Window>('/api/last/window').subscribe(data => {
      this.klimaOn = data.klimaOn;
      this.windowOpen = data.windowOpen;
      this.lastWindowUpdate = data.localDateTime;
    });
    this.windowSubscription = this._mqttService.observe('window').subscribe((message: IMqttMessage) => {
      this.parseJsonAndUpdate(message.payload.toString(),'window');
    });
    this.lightSubscription = this._mqttService.observe('light').subscribe((message: IMqttMessage) => {
      this.parseJsonAndUpdate(message.payload.toString(),'light');
    });
  }
  updateKlimaState() {
    this._mqttService.publish('window',`
      {
        "klimaOn":${!this.klimaOn},
        "windowOpen":${this.windowOpen}
      }
    `).subscribe();
  }
  updateLightState() {
    this._mqttService.publish('light', `{
      "lightOn":${!this.lightOn},
      "movementDetected":${this.movementDetected}
    }`).subscribe()
  }
  parseJsonAndUpdate(message: string,topic: String) {
    let object = JSON.parse(message);
    console.log(object,message);
    if(topic == 'window') {
      this.klimaOn = JSON.parse(object["klimaOn"]);
      this.windowOpen = JSON.parse(object["windowOpen"]);
      this.lastWindowUpdate = formatDate(new Date(),'HH:mm:ss dd MMM, yyyy','en');
      //update button
    }
    if(topic == 'light') {
      this.lightOn = JSON.parse(object["lightOn"]);
      this.movementDetected = JSON.parse(object["movementDetected"]);
      this.lastLightUpdate = formatDate(new Date(),'HH:mm:ss dd MMM, yyyy','en');
      //update button
    }
  }

}
