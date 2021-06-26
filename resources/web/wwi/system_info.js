export default class SystemInfo {
  static isMacOS() {
    // https://stackoverflow.com/questions/10527983/best-way-to-detect-mac-os-x-or-windows-computers-with-javascript-or-jquery
    return navigator.platform.toUpperCase().indexOf('MAC') >= 0;
  }

  static isIOS() {
    // https://stackoverflow.com/questions/9038625/detect-if-device-is-ios
    return !!navigator.platform && /iPad|iPhone|iPod/.test(navigator.platform);
  }

  static isMobileDevice() {
    // https://stackoverflow.com/questions/11381673/detecting-a-mobile-browser
    return /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent);
  }
}
