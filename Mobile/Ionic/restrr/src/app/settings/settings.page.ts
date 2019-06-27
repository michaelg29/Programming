import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-settings',
  templateUrl: 'settings.page.html',
  styleUrls: ['settings.page.scss'],
})
export class SettingsPage implements OnInit {

  defaultTime: any = '00:01:30';

  constructor(private router: Router, private route : ActivatedRoute ) {
    this.route.params.forEach(param => this.defaultTime = param['time']);
  }

  ngOnInit() {
  }


  save() {
    this.router.navigate(['home', this.defaultTime]);
  }
}
