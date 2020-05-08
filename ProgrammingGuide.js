import React, { Component, Fragment, createRef } from 'react';
import PropTypes from 'prop-types';
import { findNodeHandle, NativeModules, requireNativeComponent, UIManager } from 'react-native';

const { PlatformConstants } = NativeModules;

const ShadowProgrammingGuideView = requireNativeComponent('ShadowProgrammingGuideView');

class ProgrammingGuide extends Component {
  static propTypes = {
    duration: PropTypes.number.isRequired,
    schedules: PropTypes.arrayOf(PropTypes.object).isRequired,
    schedulesStartTime: PropTypes.number.isRequired,
    currentChannelIndex: PropTypes.number.isRequired,
    onHandleGuideChannelPress: PropTypes.func,
    onHandleFavoriteIconPress: PropTypes.func,
    onHandleGuideContentPress: PropTypes.func,
    onHandleGuideContentFocus: PropTypes.func,
    onHandleGuideContentBlur: PropTypes.func,
  };

  constructor(props) {
    super(props);

    this.filters = [
      { label: 'All', prop: 'isFavorite', match: false }, // All
      { label: 'Favorites', prop: 'isFavorite', match: true }, // Favorites
      { label: 'Movies', prop: 'categories', match: 'Movies' }, // Movies
      { label: 'TV Shows', prop: 'categories', match: 'TVShow' }, // TV Shows
      { label: 'Sports', prop: 'categories', match: 'Sports' }, // Sports
      { label: 'Kids', prop: 'categories', match: 'Kids' }, // Kids
    ];

    this.sortKeys = [
      { label: 'Sort ABC', prop: 'channelName', ascending: true }, // Sort by name ascending
      { label: 'Sort 123', prop: 'channelNumber', ascending: true }, // Sort by number ascending
    ];

    this.dateSelector = {
      label: 'Jump to Date', // Jump to Date
      days: 14,
    };

    this.increment = 10;
    this.delay = PlatformConstants.platform === 'android' ? 250 : 50;

    this.interval = undefined;

    this.shadowRef = createRef();
  }

  componentWillUnmount() {
    clearInterval(this.interval);
  }

  shouldComponentUpdate(nextProps) {
    if (this.skip) {
      return (this.skip = false);
    }

    if (
      nextProps.schedules instanceof Array === true &&
      this.props.schedules !== nextProps.schedules
    ) {
      this._reset(Date.now());
      this._load(nextProps.schedules);
    }

    if (this.props.currentChannelIndex !== nextProps.currentChannelIndex) {
      this._setCurrentChannel(nextProps.currentChannelIndex, nextProps.schedules);
    }

    return false;
  }

  _prepend = (channels) => {
    if (this.shadowRef.current) {
      UIManager.dispatchViewManagerCommand(
        findNodeHandle(this.shadowRef.current),
        UIManager.ShadowProgrammingGuideView.Commands.Prepend,
        [channels]
      );
    }
  };

  _append = (channels) => {
    if (this.shadowRef.current) {
      UIManager.dispatchViewManagerCommand(
        findNodeHandle(this.shadowRef.current),
        UIManager.ShadowProgrammingGuideView.Commands.Append,
        [channels]
      );
    }
  };

  _reset = (schedulesStartTime) => {
    if (this.shadowRef.current && schedulesStartTime instanceof Number) {
      const startTime = schedulesStartTime / 1000;

      UIManager.dispatchViewManagerCommand(
        findNodeHandle(this.shadowRef.current),
        UIManager.ShadowProgrammingGuideView.Commands.Reset,
        [startTime]
      );
    }
  };

  _load = (schedules) => {
    clearInterval(this.interval);

    this.prependPosition = this.props.currentChannelIndex;
    this.prependOffset = this.props.currentChannelIndex;
    this.appendPosition = this.props.currentChannelIndex;
    this.appendOffset = this.props.currentChannelIndex;

    this.interval = setInterval(() => {
      this.prependOffset -= this.increment;
      this.appendOffset += this.increment;

      if (this.prependOffset < 0) {
        this.prependOffset = 0;
      }

      this._prepend(schedules.slice(this.prependOffset, this.prependPosition));
      this.prependPosition = this.prependOffset;

      if (this.appendOffset > schedules.length) {
        this.appendOffset = schedules.length;
      }

      this._append(schedules.slice(this.appendPosition, this.appendOffset));
      this.appendPosition = this.appendOffset;

      if (this.appendPosition === schedules.length && this.prependPosition === 0) {
        clearInterval(this.interval);
      }
    }, this.delay);
  };

  _setCurrentChannel = (currentChannelIndex, schedules) => {
    if (this.shadowRef.current) {
      UIManager.dispatchViewManagerCommand(
        findNodeHandle(this.shadowRef.current),
        UIManager.ShadowProgrammingGuideView.Commands.SetCurrentChannel,
        [{ channelId: schedules[currentChannelIndex]?.channelId }]
      );
    }
  };

  _channelSelector = (asset) =>
    this.props.schedules.find((row) => {
      return row.channelId === asset.channelId;
    });

  _assetSelector = (asset) =>
    this._channelSelector(asset)?.contents.find((item) => {
      return item.resourceId === asset.resourceId;
    });

  _handleGuideContentFocus = (event) => {
    const asset = this._assetSelector(event.nativeEvent);

    if (this.props.onHandleGuideContentFocus) {
      this.props.onHandleGuideContentFocus(asset);
    }
  };

  _handleGuideContentBlur = (event) => {
    const asset = this._assetSelector(event.nativeEvent);

    if (this.props.onHandleGuideContentBlur) {
      this.props.onHandleGuideContentBlur(asset);
    }
  };

  _handleGuideContentPress = (event) => {
    const asset = this._assetSelector(event.nativeEvent);

    if (this.props.onHandleGuideContentPress) {
      this.props.onHandleGuideContentPress(asset);
    }
  };

  _handleGuideChannelPress = (event) => {
    const channel = this._channelSelector(event.nativeEvent);

    if (this.props.onHandleGuideChannelPress) {
      this.props.onHandleGuideChannelPress(channel.channelId);
    }
  };

  _handleFavoriteIconPress = (event) => {
    const channel = this._channelSelector(event.nativeEvent);

    this.skip = true;

    if (this.props.onHandleFavoriteIconPress) {
      this.props.onHandleFavoriteIconPress(channel.channelId);
    }
  };

  _handleOnLayout = (event) => {
    const { layout } = event.nativeEvent;

    if (layout && this.props.schedules instanceof Array) {
      this._load(this.props.schedules);
    }
  };

  render() {
    const { currentChannelIndex, schedules } = this.props;

    return (
      <Fragment>
        <ShadowProgrammingGuideView
          ref={this.shadowRef}
          filters={this.filters}
          sortKeys={this.sortKeys}
          dateSelector={this.dateSelector}
          {...this.props}
          currentChannelIndex={{ channelId: schedules[currentChannelIndex]?.channelId }}
          onHandleGuideChannelPress={this._handleGuideChannelPress} //eslint-disable-line react/jsx-handler-names
          onHandleFavoriteIconPress={this._handleFavoriteIconPress} //eslint-disable-line react/jsx-handler-names
          onHandleGuideContentPress={this._handleGuideContentPress} //eslint-disable-line react/jsx-handler-names
          onHandleGuideContentFocus={this._handleGuideContentFocus} //eslint-disable-line react/jsx-handler-names
          onHandleGuideContentBlur={this._handleGuideContentBlur} //eslint-disable-line react/jsx-handler-names
          onLayout={this._handleOnLayout} //eslint-disable-line react/jsx-handler-names
        />
      </Fragment>
    );
  }
}

export default ProgrammingGuide;
