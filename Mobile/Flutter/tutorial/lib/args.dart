library tutorial.args;

import 'package:flutter/material.dart';

class Args {
  final String username;
  final String password;

  Args(this.username, this.password);
}

class ArgsScreen extends StatelessWidget {
  static const routeName = '/extractArguments';

  final Map<String, dynamic> args;

  ArgsScreen({Key key, @required this.args}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: null,
      body: Center(
        child: Text(args["password"]),
      ),
    );
  }
}
