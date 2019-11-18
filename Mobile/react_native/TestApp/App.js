import React, { Component } from 'react';
import { StyleSheet, Text, View, Image, TextInput, Button, ScrollView, FlatList, SectionList } from 'react-native';

class Bananas extends Component {
  render() {
    let pic = {
      url: 'https://upload.wikimedia.org/wikipedia/commons/d/de/Bananavarieties.jpg'
    };

    return (
      <Image source={require('./assets/icon.png')} style={{ width: 193, height: 100 }}/>
    );
  }
}

class Greeting extends Component {
  render() {
    return (
      <Text style={[styles.smallText, styles.greetingText]}>Hello, {this.props.name}!</Text>
    );
  }
}

class Blink extends Component {
  componentDidMount() {
    // toggle every second
    setInterval(() => {
      this.setState(previousState => (
        { isShowingText: !previousState.isShowingText }
      ))
    }, 1000);
  }

  state = { isShowingText: true };

  render() {
    if (!this.state.isShowingText) {
      return null;
    }

    return (
      <Text>{this.props.text}</Text>
    );
  }
}

class PizzaTranslator extends Component {
  constructor(props) {
    super(props);
    this.state = { text: '' };
  }

  render() {
    return (
      <View style={{padding: 10}}>
        <TextInput
          style= {{height: 40, borderColor: 'black', borderWidth: 1}}
          placeholder="Type here to translate"
          onChangeText={(text) => this.setState({text})}
          value={this.state.text}
        />

        <Text style={{padding: 10, fontSize: 42}}>
          {this.state.text.split(' ').map((word) => word && '🍕').join(' ')}
        </Text>
      </View>
    );
  }
}

class ButtonBasics extends Component {
  _onPressButton() {
    alert("You tapped the button!");
  }

  render() {
    return (
      <View style={styles.container}>
        <View style={styles.buttonContainer}>
          <Button
            onPress={this._onPressButton}
            title="Press Me"
          />
        </View>
        <View style={styles.buttonContainer}>
          <Button
            onPress={this._onPressButton}
            title="Press Me"
            color="#841584"
          />
        </View>
        <View style={styles.alternativeLayoutButtonContainer}>
          <Button
            onPress={this._onPressButton}
            title="This looks great!"
          />
          <Button
            onPress={this._onPressButton}
            title="OK!"
            color="#841584"
          />
        </View>
      </View>
    )
  }
}

class FlatListTest extends Component {
  render() {
    return (
      <FlatList
        data={this.props.data}
        renderItem={({item}) => <Text style={styles.item}>{item.name}</Text>}
        keyExtractor={(item, index) => index.toString()}
      />
    );
  }
}

class SectionListTest extends Component {
  render() {
    return (
      <SectionList
        sections={this.props.data}
        renderItem={({item}) => <Text style={styles.item}>{item}</Text>}
        renderSectionHeader={({section}) => <Text style={styles.sectionHeader}>{section.title}</Text>}
        keyExtractor={(item, index) => index}
      />
    );
  }
}

let people = [ 
  { name: 'Michael'},
  { name: 'Mao' },
  { name: 'Joseph' },
  { name: 'Karl' },
];

let peopleSorted = [
  {title: 'J', data: ['Joseph']},
  {title: 'K', data: ['Karl']},
  {title: 'M', data: ['Michael', 'Mao']},
];

export default class HelloWorldApp extends Component {
  render() {
    return (
      <ScrollView>
        <View style={styles.container}>
          <Greeting name="Michael"/>
          <Text>Look at this image:</Text>
          <Blink text="Watch me blink"/>
          <Bananas/>
          <PizzaTranslator/>
          <ButtonBasics/>
          <Text style={{fontSize: 96}}>Please scroll</Text>
          
          <View style={styles.hr}/>

          <FlatListTest data={people}/>

          <View style={styles.hr}/>

          <SectionListTest data={peopleSorted}/>
        </View>
      </ScrollView>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    /*
    https://facebook.github.io/react-native/docs/flexbox
    flexDirection: 'row' | 'row-reverse' | 'column' | 'column-reverse',
    */
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  greetingText: {
    fontSize: 36,
    fontWeight: 'bold',
  },
  smallText: {
    fontSize: 10,
  },
  buttonContainer: {
    margin: 20
  },
  alternativeLayoutButtonContainer: {
    margin: 20,
    flexDirection: 'row',
    justifyContent: 'space-between'
  },
  item: {
    padding: 10,
    fontSize: 18,
    height: 44,
  },
  hr: {
    borderBottomColor: '#000',
    borderBottomWidth: 3,
  },
  sectionHeader: {
    paddingTop: 2,
    paddingLeft: 10,
    paddingRight: 10,
    paddingBottom: 2,
    fontSize: 14,
    fontWeight: 'bold',
    backgroundColor: 'rgba(247,247,247,1.0)',
  },
  item: {
    padding: 10,
    fontSize: 18,
    height: 44,
  },
});
