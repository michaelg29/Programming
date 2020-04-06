library tutorial.nav;

import 'package:flutter/material.dart';

Map<String, Map<String, dynamic>> routes;

void setRoutes(List<List<dynamic>> newRoutes) {
  newRoutes.forEach((item) {
    routes[item[0]] = {
      "name": item[1],
      "func": item[2],
    };
  });
}

void getName(String route) {
  return routes[route]["name"];
}

Widget buildList(BuildContext context,
    [Map<String, Map<String, dynamic>> args]) {
  return ListView.builder(
    padding: const EdgeInsets.all(16.0),
    itemBuilder: (context, i) {
      String rt = routes.keys.elementAt(i);
      return ListTile(
        title: Text(routes[rt]["name"]),
        onTap: () {
          if (args.containsKey(rt)) {
            sendTo(context, rt, args[rt]);
          } else {
            sendTo(context, rt);
          }
        },
      );
    },
  );
}

void sendTo(BuildContext context, String route, [Map<String, dynamic> args]) {
  Navigator.pop(context);

  Widget result;

  if (args == null) {
    result = routes[route]["func"](context);
  } else {
    result = routes[route]["func"](context, args);
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
