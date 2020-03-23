import React from 'react';
import { requireNativeComponent } from 'react-native';

const ShadowProgrammingGuideView = requireNativeComponent('ShadowProgrammingGuideView');

export default class ProgrammingGuide extends React.PureComponent {
  render() {
    return <ShadowProgrammingGuideView {...this.props} />;
  }
}