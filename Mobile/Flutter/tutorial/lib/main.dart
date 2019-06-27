import 'package:flutter/material.dart';
import 'package:english_words/english_words.dart'

//https://flutter.dev/docs/get-started/codelab#step-4-create-an-infinite-scrolling-listview


import 'package:flutter/rendering.dart';void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: "Welcome to Flutter",
      home: Scaffold(
          appBar: AppBar(
            title: Text("Welcome to Flutter"),
          ),
          body: Center(
            child: RandomWords(),
          )),
    );
  }
}

class RandomWordsState extends State<RandomWords> {
  @override
  Widget build(BuildContext context) {
    final wordPair = WordPair.random();
    return Text(wordPair.asPascalCase);
  }
}

class RandomWords extends StatefulWidget {
  @override
  RandomWordsState createState() => RandomWordsState();
}