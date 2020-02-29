import 'package:flutter/material.dart';
import 'package:english_words/english_words.dart';

import 'globals.dart' as globals;

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: "Word Generator",
      theme: ThemeData(
        primaryColor: Colors.white,
      ),
      home: RandomWords(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class RandomWords extends StatefulWidget {
  @override
  RandomWordsState createState() => RandomWordsState();
}

class RandomWordsState extends State<RandomWords> {
  final _suggestions = <WordPair>[]; // private variable
  final _biggerFont = const TextStyle(fontSize: 18.0);

  Widget _buildSuggestions() {
    return ListView.builder(
        padding: const EdgeInsets.all(16.0),
        itemBuilder: (context, i) {
          // callback
          if (i.isOdd) return Divider();

          final index = i ~/ 2; // integer division
          if (index >= _suggestions.length) {
            _suggestions.addAll(
                generateWordPairs().take(10)); // generate more if reach end
          }
          return _buildRow(_suggestions[index]);
        });
  }

  Widget _buildRow(WordPair pair) {
    final bool alreadySaved = globals.saved.contains(pair);

    return ListTile(
      title: Text(
        pair.asPascalCase,
        style: _biggerFont,
      ),
      trailing: Icon(
        alreadySaved ? Icons.favorite : Icons.favorite_border,
        color: alreadySaved ? Colors.red : null,
      ),
      onTap: () {
        setState(() {
          if (alreadySaved) {
            globals.saved.remove(pair);
          } else {
            globals.saved.add(pair);
          }
        });
      },
    );
  }

  Route _savedRoute() {
    return PageRouteBuilder(
      pageBuilder: (context, animation, secondaryAnimation) => SavedPage(),
      transitionsBuilder: (context, animation, secondaryAnimation, child) {
        var begin = Offset(0.0, 1.0);
        var end = Offset.zero;
        var curve = Curves.ease;
        var tween =
            Tween(begin: begin, end: end).chain(CurveTween(curve: curve));
        var offsetAnimation = animation.drive(tween);

        return SlideTransition(position: offsetAnimation, child: child);
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    final wordPair = WordPair.random();
    return Scaffold(
      appBar: AppBar(
        title: Text("Name Generator"),
        actions: <Widget>[
          IconButton(
            icon: Icon(Icons.list),
            onPressed: () {
              Navigator.of(context).push(_savedRoute());
            },
          ),
        ],
      ),
      body: _buildSuggestions(),
    );
  }
}

class SavedPage extends StatelessWidget {
  final _biggerFont = const TextStyle(fontSize: 18.0);

  Widget build(BuildContext context) {
    final Iterable<ListTile> tiles = globals.saved.map(
      (WordPair pair) {
        return ListTile(
            title: Text(
          pair.asPascalCase,
          style: _biggerFont,
        ));
      },
    );
    final List<Widget> divided = ListTile.divideTiles(
      context: context,
      tiles: tiles,
    ).toList();

    return Scaffold(
      appBar: AppBar(
        title: Text("Saved Suggestions"),
      ),
      body: ListView(children: divided),
    );
  }
}
