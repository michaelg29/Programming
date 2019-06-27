import { Component, OnInit } from '@angular/core';
import { Router, NavigationExtras } from '@angular/router';

@Component({
  selector: 'app-settings',
  templateUrl: 'settings.page.html',
  styleUrls: ['settings.page.scss'],
})
export class SettingsPage implements OnInit {

  defaultTime: any = '00:01:30';

  constructor(private router: Router) { }

  ngOnInit() {
  }


  save() {
    this.router.navigate(['home', { time: this.defaultTime }]);
  }
}
