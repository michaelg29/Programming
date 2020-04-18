import "package:flutter/material.dart";

import "utils/navigator.dart" as nav;
import "assets/style/styles.dart" as styles;
import "utils/fstream.dart" as fstream;

import "home.dart" as home;
import "enter.dart" as enter;
import "chat.dart" as chat;

void main() => runApp(ChatApp());

List<List<dynamic>> routes = [
  [
    "/",
    "Home",
    (context) => home.HomePage(),
  ],
  [
    "/enter",
    "Enter Chat Code",
    (context) => enter.EnterPage(),
  ],
  [
    "/chat",
    "Chat",
    (context, args) => chat.ChatPage(args: args),
  ],
];

class ChatApp extends StatelessWidget {
  ChatApp() {
    nav.setRoutes(routes);
  }

  @override
  Widget build(BuildContext context) {
    fstream.writeApiKey("AIzaSyD76y9mfnTewCq6Qj52svrNdPMVMODa328");

    return MaterialApp(
      title: "YouTube Streaming Chat",
      theme: styles.main,
      debugShowCheckedModeBanner: false,
      home: home.HomePage(),
    );
  }
}
