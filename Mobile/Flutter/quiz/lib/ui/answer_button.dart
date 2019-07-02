import 'package:flutter/material.dart';

class AnswerButton extends StatelessWidget {
  // must be final because immutable class (stateless is static)
  final bool _answer;
  final VoidCallback _onTap; // callback to function

  AnswerButton(this._answer, this._onTap);

  @override
  Widget build(BuildContext context) {
    return new Expanded(
      child: new Material( // false button
        color: _answer ? Colors.greenAccent : Colors.redAccent,
        child: new InkWell(
          onTap: () => _onTap(),
          child: new Center(
            child: new Container(
              decoration: new BoxDecoration(
                border: new Border.all(
                  color: Colors.white,
                  width: 5.0,
                ),
              ),
              padding: new EdgeInsets.all(20.0),
              child: new Text(
                _answer ? "True" : "False",
                style: new TextStyle(
                  color: Colors.white,
                  fontSize: 40.0,
                  fontWeight: FontWeight.bold,
                  fontStyle: FontStyle.italic,
                ),  
              ),
            ),
          ),
        ),
      ),
    );
  }
}