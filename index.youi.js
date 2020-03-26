/**
 * Basic You.i RN app
 */
import React, { Component } from "react";
import { AppRegistry, View } from "react-native";

import ProgrammingGuide from './ProgrammingGuide';

const schedules = require('./schedules.json');

export default class YiReactApp extends Component {
  render() {
    return (
      <View style={{ flex: 1 }}>
        <ProgrammingGuide onLayout={() => console.log('ProgrammingGuide', 'onLayout')} schedules={schedules}/>
      </View>
    );
  }
}

AppRegistry.registerComponent("YiReactApp", () => YiReactApp);
