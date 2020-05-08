/**
 * Basic You.i RN app
 */
import React, { Component } from "react";
import { AppRegistry } from "react-native";

import ProgrammingGuide from './ProgrammingGuide';

const data = require('./schedules.json');

export default class YiReactApp extends Component {
  render() {
    const startTime = new Date('2020-03-23T16:00:00Z').getTime() / 1000;

    return (
      <ProgrammingGuide schedulesStartTime={startTime} currentChannelIndex={5} schedules={data}/>
    );
  }
}

AppRegistry.registerComponent("YiReactApp", () => YiReactApp);
