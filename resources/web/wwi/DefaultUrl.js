'use strict';

export default class DefaultUrl {
  static wwiUrl() {
    if (typeof this._wwiUrl === 'undefined') {
      this._wwiUrl = '';
      const scripts = document.getElementsByTagName('script');
      for (let i = scripts.length - 1; i >= 0; i--) {
        let src = scripts[i].src;
        if (src.indexOf('?') > 0)
          src = src.substring(0, src.indexOf('?'));
        if (src.endsWith('init_animation.js') || src.endsWith('enum.js') || src.endsWith('wrenjs.js')) {
          this._wwiUrl = src.substring(0, src.lastIndexOf('/')); // remove "wrenjs.js"
          break;
        }
      }
    }
    return this._wwiUrl;
  }

  static wwiImagesUrl() {
    return this.wwiUrl() + '/images/';
  }

  static wrenImagesUrl() {
    return this.wwiUrl() + '/images/post_processing/';
  }
};
