library tutorial.counter_file;

import 'package:flutter/material.dart';
import "fstream.dart" as fstream;
import "dart:io";

class CounterFileScreen extends StatefulWidget {
  @override
  CounterState createState() => CounterState();
}

class CounterState extends State<CounterFileScreen> {
  int _counter;

  @override
  void initState() {
    super.initState();
    fstream.readCounter().then((int value) {
      setState(() {
        _counter = value;
      });
    });
  }

  Future<File> _incrementCounter() {
    setState(() {
      _counter++;
    });

    return fstream.writeCounter(_counter);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Read/Write Counter")),
      body: Center(
        child: Text(
          "Button tapped $_counter time${_counter == 1 ? '' : 's'}",
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: "Increment",
        child: Icon(Icons.add),
      ),
    );
  }
}
