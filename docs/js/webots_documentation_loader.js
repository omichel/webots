function loadScript(scriptUrl) {
  return new Promise(function(resolve, reject) {
    let script = document.createElement('script');
    script.onload = resolve;
    script.src = scriptUrl;
    document.head.appendChild(script);
  });
}

async function init() {
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

  let promises = [];
  promises.push(loadScript('https://git.io/glm-js.min.js'));
  promises.push(loadScript('https://cyberbotics.com/wwi/R2021b/enum.js'));
  promises.push(loadScript('https://cyberbotics.com/wwi/R2021b/wrenjs.js'));

  await Promise.all(promises);
  script = document.createElement('script');
  script.src = 'https://cyberbotics.com/wwi/R2021b/viewer.js';
  script.type = 'module';
  document.head.appendChild(script);
}

init();
