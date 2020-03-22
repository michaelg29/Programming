import 'package:flutter/material.dart';
import 'package:english_words/english_words.dart';

import 'globals.dart' as globals;
import 'style.dart' as styles;
import "fstream.dart" as fstream;

import 'json_photos.dart' as json_data;
import 'args.dart' as argsDisplay;
import "counterFile.dart" as counter_file;

import "navigator.dart" as nav;

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  MyApp() {
    nav.setRoutes({
      "/": (context) => RandomWords(),
      "/saved": (context) => SavedPage(),
      "/tabs": (context) => TabbedPage(),
      "/form": (context) => FormPage(),
      "/json": (context) => json_data.JsonPage(),
      "/counter": (context) => counter_file.CounterFileScreen(),
      argsDisplay.ArgsScreen.routeName: (context, args) =>
          argsDisplay.ArgsScreen(args: args),
    });
  }

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

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Name Generator"),
        actions: <Widget>[
          IconButton(
            icon: Icon(Icons.list),
            onPressed: () {
              nav.sendTo(context, "/saved");
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
                nav.sendTo(context, "/saved");
              },
            ),
            ListTile(
              title: Text("Tabs"),
              onTap: () {
                //Navigator.pop(context);
                //Navigator.of(context).push(_getRoute(TabbedPage()));

                nav.sendTo(context, "/tabs");
              },
            ),
            ListTile(
              title: Text("Form"),
              onTap: () {
                nav.sendTo(context, "/form");
              },
            ),
            ListTile(
              title: Text("JSON"),
              onTap: () {
                nav.sendTo(context, "/json");
              },
            ),
            ListTile(
              title: Text("Counter File"),
              onTap: () {
                nav.sendTo(context, "/counter");
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
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.backspace),
        onPressed: () {
          nav.sendTo(context, "/");
        },
      ),
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
  final unController = TextEditingController();

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
    unController.dispose();
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
                // onChanged: (text) {
                //   print("Username: $text");
                // },
                autofocus: true,
                controller: unController,
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
                      nav.sendTo(context, argsDisplay.ArgsScreen.routeName, {
                        "username": unController.text,
                        "password": pwdController.text
                      });

                      // Navigator.pushNamed(
                      //   context,
                      //   args.ArgsScreen.routeName,
                      //   arguments:
                      //       args.Args(unController.text, pwdController.text),
                      // );

                      // return showDialog(
                      //   context: context,
                      //   builder: (context) {
                      //     return AlertDialog(
                      //       content: Text("Password: ${pwdController.text}"),
                      //     );
                      //   },
                      // );
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
