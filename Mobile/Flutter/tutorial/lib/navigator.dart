library tutorial.nav;

import 'package:flutter/material.dart';

import "counterFile.dart" as c;

Map<String, Function> routes;

void setRoutes(Map<String, Function> newRoutes) {
  routes = newRoutes;
}

void sendTo(BuildContext context, String route, [Map<String, dynamic> args]) {
  Navigator.pop(context);

  Widget result;

  if (args == null) {
    result = routes[route](context);
  } else {
    result = routes[route](context, args);
  }

  Navigator.of(context).push(PageRouteBuilder(
    pageBuilder: (context, animation, secondaryAnimation) => result,
    transitionsBuilder: (context, animation, secondaryAnimation, child) {
      var begin = Offset(0.0, 1.0);
      var end = Offset.zero;
      var curve = Curves.ease;
      var tween = Tween(begin: begin, end: end).chain(CurveTween(curve: curve));
      var offsetAnimation = animation.drive(tween);

      return SlideTransition(position: offsetAnimation, child: child);
    },
  ));
}
