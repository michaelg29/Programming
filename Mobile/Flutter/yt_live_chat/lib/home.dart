import "package:flutter/material.dart";

import "utils/navigator.dart" as nav;

class HomePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        RaisedButton(
          child: Text("Enter Stream ID"),
          onPressed: () {
            nav.sendTo(context, "/enter");
          },
        ),
      ],
    );
  }
}
