import { Component } from '@angular/core';
import { Insomnia } from '@ionic-native/insomnia/ngx';
import { NavController, NavParams } from '@ionic/angular';
import { Router, NavigationExtras } from '@angular/router';

import { SettingsPage } from '../settings/settings.page';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  progress:any = 0;
  percent: number = 0;
  radius: number = 100;
  minutes: number = 1;
  seconds: any = 10;
  timer: any = false;
  fullTime: any = '00:01:30';

  elapsed: any = {
    h: '00',
    m: '00',
    s: '00'
  };

  overallTimer: any = false;

  settingsPage = SettingsPage;

  constructor(private insomnia: Insomnia, private router : Router, public navExtras : NavigationExtras ) {
    console.log(navExtras);
  }

  startTimer() {
    if (this.timer) {
      clearInterval(this.timer);
    }

    if (!this.overallTimer) {
      this.progressTimer();
      this.insomnia.keepAwake();
    }

    this.timer = false;
    this.percent = 0;
    this.progress = 0;

    let timeSplit = this.fullTime.split(':');
    this.minutes = timeSplit[1];
    this.seconds = timeSplit[2];

    let totalSeconds = Math.floor(this.minutes * 60) + parseInt(this.seconds);

    this.timer = setInterval(() => {
      if (this.percent == this.radius) clearInterval(this.timer)
      this.percent = Math.floor((this.progress / totalSeconds) * 100)
      ++this.progress
    }, 1000);
  }

  progressTimer() {
    let countDownDate = new Date();

    this.overallTimer = setInterval(() => {
      let now = new Date().getTime();

      let distance = now - countDownDate.getTime();

      this.elapsed.h = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
      this.elapsed.m = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
      this.elapsed.s = Math.floor((distance % (1000 * 60)) / 1000);

      this.elapsed.h = this.pad(this.elapsed.h, 2);
      this.elapsed.m = this.pad(this.elapsed.m, 2);
      this.elapsed.s = this.pad(this.elapsed.s, 2);
    });
  }

  pad(num, size) {
    let s = num+"";
    while (s.length < size) s = "0" + s;
    return s;
  }

  stopTimer() {
    clearInterval(this.timer);
    clearInterval(this.overallTimer);
    this.overallTimer = false;
    this.timer = false;    
    this.percent = 0;
    this.progress = 0;
    this.elapsed = {
      h: '00',
      m: '00',
      s: '00'
    }
    this.insomnia.allowSleepAgain()
  }

  settings() {
    this.router.navigate(['settings', { time: this.fullTime }]);
  }
}
