import { NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';

const routes: Routes = [
  { path: '', redirectTo: 'home/00:01:30', pathMatch: 'full' },
  { path: 'home/:time', loadChildren: './home/home.module#HomePageModule' },
  { path: 'settings/:time', loadChildren: './settings/settings.module#SettingsPageModule' },
];

@NgModule({
  imports: [
    RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules })
  ],
  exports: [RouterModule]
})
export class AppRoutingModule { }
