import 'package:flutter/material.dart';

import '../utils/question.dart';
import '../utils/quiz.dart';

import '../ui/answer_button.dart';
import '../ui/question_text.dart';
import '../ui/question_overlay.dart';

import 'score_page.dart';

class QuizPage extends StatefulWidget { // states change and update
  @override
  State createState() => new QuizPageState();
}

class QuizPageState extends State<QuizPage> {

  Question currentQuestion;
  Quiz quiz = new Quiz([
    new Question("Elon Musk is human", false),
    new Question("Pizza is healthy", false),
    new Question("Flutter is awesome", true),
  ]);
  bool isCorrect;
  bool overlayShouldBeVisible = false;

  @override
  void initState() {
    super.initState();
    currentQuestion = quiz.nextQuestion;
  }

  void handleAnswer(bool answer) {
    isCorrect = currentQuestion.answer == answer;
    quiz.answer(isCorrect);
    this.setState(() { // results in rebuilding widget
      overlayShouldBeVisible = true;
    });
  }

  @override
  Widget build(BuildContext context) {
    return new Stack(
      fit: StackFit.expand,
      children: <Widget>[
        new Column( // main page
          children: <Widget>[
            new AnswerButton(true, () => this.handleAnswer(true) ), // true button
            new QuestionText(currentQuestion.question, quiz.questionNumber),
            new AnswerButton(false, () => this.handleAnswer(false) ), // false button
          ],
        ),
        overlayShouldBeVisible ? new QuestionOverlay(
          isCorrect,
          () {
            if (quiz.length == quiz.questionNumber) {
              Navigator.of(context).pushAndRemoveUntil(new MaterialPageRoute(builder: (BuildContext context) => new ScorePage(quiz.score, quiz.length)), (Route route) => route == null);
              // cannot go back, clear previous routes until null
              return;
            }
            currentQuestion = quiz.nextQuestion;
            this.setState(() {
              overlayShouldBeVisible = false;
            });
          }
        ) : new Container(),
      ],
    );
  }
}