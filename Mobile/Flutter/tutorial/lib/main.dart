import 'package:flutter/material.dart';
import 'package:english_words/english_words.dart';

import 'globals.dart' as globals;
import 'style.dart' as styles;

import 'json_photos.dart' as json_data;

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: "Word Generator",
      theme: styles.main,
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

  Route _getRoute(Widget route) {
    return PageRouteBuilder(
      pageBuilder: (context, animation, secondaryAnimation) => route,
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
              Navigator.of(context).push(_getRoute(SavedPage()));
            },
          ),
        ],
      ),
      body: _buildSuggestions(),
      drawer: Drawer(
        child: ListView(
          padding: EdgeInsets.zero,
          children: <Widget>[
            DrawerHeader(
              child: Text("Pages"),
              decoration: BoxDecoration(
                color: Colors.blue,
              ),
            ),
            ListTile(
              title: Text("Saved Items"),
              onTap: () {
                Navigator.pop(context);
                Navigator.of(context).push(_getRoute(SavedPage()));
              },
            ),
            ListTile(
              title: Text("Tabs"),
              onTap: () {
                Navigator.pop(context);
                Navigator.of(context).push(_getRoute(TabbedPage()));
              },
            ),
            ListTile(
              title: Text("Form"),
              onTap: () {
                Navigator.pop(context);
                Navigator.of(context).push(_getRoute(FormPage()));
              },
            ),
            ListTile(
              title: Text("JSON"),
              onTap: () {
                Navigator.pop(context);
                Navigator.of(context).push(_getRoute(json_data.JsonPage()));
              },
            ),
          ],
        ),
      ),
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

class TabbedPage extends StatelessWidget {
  Widget build(BuildContext context) {
    return Scaffold(
      body: DefaultTabController(
        length: 3,
        child: Scaffold(
          appBar: AppBar(
            title: Text("Tabs example"),
            bottom: TabBar(
              tabs: [
                Tab(icon: Icon(Icons.directions_car)),
                Tab(icon: Icon(Icons.directions_transit)),
                Tab(icon: Icon(Icons.directions_bike)),
              ],
            ),
          ),
          body: TabBarView(
            children: [
              Center(
                child: Builder(
                  builder: (context) => IconButton(
                    icon: Icon(Icons.directions_car),
                    onPressed: () {
                      final snackBar = SnackBar(
                        content: Text("Car directions retrieval"),
                        action: SnackBarAction(
                          label: "Cancel",
                          onPressed: () {},
                        ),
                      );

                      Scaffold.of(context).showSnackBar(snackBar);
                    },
                  ),
                ),
              ),
              Icon(Icons.directions_transit),
              Icon(Icons.directions_bike),
            ],
          ),
        ),
      ),
    );
  }
}

class FormPage extends StatefulWidget {
  @override
  FormPageState createState() {
    return FormPageState();
  }
}

class FormPageState extends State<FormPage> {
  final _formKey = GlobalKey<FormState>();
  final pwdController = TextEditingController();

  FocusNode pwdNode;

  @override
  void initState() {
    super.initState();
    pwdController.addListener(_printVal);

    pwdNode = FocusNode();
  }

  @override
  void dispose() {
    pwdNode.dispose();
    pwdController.dispose();
    super.dispose();
  }

  _printVal() {
    //print("PWD: ${pwdController.text}");
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Form example"),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Form(
          key: _formKey,
          child: Column(
            children: <Widget>[
              TextFormField(
                validator: (value) {
                  if (value.isEmpty) {
                    return "Please enter text";
                  }
                  return null;
                },
                decoration: InputDecoration(
                  labelText: "Enter your username",
                  hintText: "Username",
                ),
                onChanged: (text) {
                  print("Username: $text");
                },
                autofocus: true,
              ),
              TextFormField(
                validator: (value) {
                  if (value != "abcd") {
                    return "Incorrect password";
                  }
                  return null;
                },
                obscureText: true,
                decoration: InputDecoration(
                  labelText: "Enter password",
                  hintText: "Password",
                ),
                controller: pwdController,
                focusNode: pwdNode,
              ),
              Builder(
                builder: (context) => RaisedButton(
                  onPressed: () {
                    if (_formKey.currentState.validate()) {
                      return showDialog(
                        context: context,
                        builder: (context) {
                          return AlertDialog(
                            content: Text("Password: ${pwdController.text}"),
                          );
                        },
                      );
                    }
                  },
                  child: Text("Submit"),
                ),
              ),
            ],
          ),
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          FocusScope.of(context).requestFocus(pwdNode);
        },
        tooltip: "Edit password",
        child: Icon(Icons.edit),
      ),
    );
  }
}
