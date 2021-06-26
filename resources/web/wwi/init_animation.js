import {webots} from './webots.js';

function init() {
  let name = document.getElementsByTagName('webots-animation')[0].title;
  if (!name)
    name = location.pathname.substring(location.pathname.lastIndexOf('/') + 1).replace('.html', '');

  const view = new webots.View(document.getElementById('view3d'));
  view.open(name + '.x3d');
  view.setAnimation(name + '.json', 'play', true);
}

init();
