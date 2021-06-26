const template = document.createElement('template');

template.innerHTML = `
<link type="text/css" href="https://cyberbotics.com/wwi/R2021b/css/animation.css" rel="stylesheet"/>
<div id="view3d" style="height:100%; width:100%"></div>
`;

export default class WebotsAnimation extends HTMLElement {
  constructor() {
    super();
    this.appendChild(template.content.cloneNode(true));

    let script = document.createElement('script');
    script.textContent = `var Module = [];
        Module['locateFile'] = function(path, prefix) {

        // if it's a data file, use a custom dir
        if (path.endsWith(".data"))
          return "https://cyberbotics.com/wwi/R2021b/" + path;

        // otherwise, use the default, the prefix (JS file's dir) + the path
        return prefix + path;
      }`;
    document.head.appendChild(script);

    this._init();
  }

  _load(scriptUrl) {
    return new Promise(function(resolve, reject) {
      let script = document.createElement('script');
      script.onload = resolve;
      script.src = scriptUrl;
      document.head.appendChild(script);
    });
  }

  async _init() {
    let promises = [];
    promises.push(this._load('https://git.io/glm-js.min.js'));
    promises.push(this._load('https://cyberbotics.com/wwi/R2021b/enum.js'));
    promises.push(this._load('https://cyberbotics.com/wwi/R2021b/wrenjs.js'));

    await Promise.all(promises);
    let script = document.createElement('script');
    script.src = 'https://cyberbotics.com/wwi/R2021b/init_animation.js';
    script.type = 'module';
    document.head.appendChild(script);
  }
}

window.customElements.define('webots-animation', WebotsAnimation);
