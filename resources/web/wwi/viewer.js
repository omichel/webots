/* eslint no-extend-native: ["error", { "exceptions": ["String"] }] */
/* global setup */
/* global showdown */
/* global hljs */

'use strict';

import {getGETQueryValue, getGETQueriesMatchingRegularExpression} from './request_methods.js';
import {webots} from './webots.js';

import WbImageTexture from './nodes/WbImageTexture.js';
import WbPBRAppearance from './nodes/WbPBRAppearance.js';
import WbShape from './nodes/WbShape.js';
import WbSphere from './nodes/WbSphere.js';
import WbTransform from './nodes/WbTransform.js';
import WbVector3 from './nodes/utils/WbVector3.js';
import WbVector4 from './nodes/utils/WbVector4.js';
import WbWorld from './nodes/WbWorld.js';
import {quaternionToVec4, vec4ToQuaternion, getAnId} from './nodes/utils/utils.js';

let handle;
let webotsView;
let pointer;
let imageTexture;
let sizeOfMarker;

if (typeof String.prototype.startsWith !== 'function') {
  String.prototype.startsWith = function(prefix) {
    return this.slice(0, prefix.length) === prefix;
  };
}

if (typeof String.prototype.endsWith !== 'function') {
  String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1;
  };
}

function isInternetExplorer() {
  const userAgent = navigator.userAgent;
  return userAgent.indexOf('MSIE') !== -1 || userAgent.indexOf('Trident') !== -1;
};

const localSetup = (typeof setup === 'undefined') ? {} : setup;
const isCyberboticsUrl = location.href.indexOf('cyberbotics.com/doc') !== -1;

function setupCyberboticsUrl(url) {
  localSetup.book = 'guide';
  localSetup.page = 'index';
  localSetup.anchor = '';
  if (!localSetup.tabs)
    localSetup.tabs = {};

  let m = url.match(new RegExp('/([^/]+)/([^/\\?#]+)([^/]*)$'));
  if (m) {
    localSetup.book = m[1];
    localSetup.page = m[2];
    const args = m[3];

    m = url.match(/version=([^&#]*)/);
    if (m) {
      const version = m[1];
      const n = version.indexOf(':');
      if (n === -1)
        localSetup.branch = version;
      else
        localSetup.branch = version.substr(n + 1);
    }

    // Extract tab options
    const tabRegex = /[?&](tab-[^=]+)=([^&#]+)/g;
    while ((m = tabRegex.exec(url)) !== null)
      localSetup.tabs[m[1]] = m[2];

    m = args.match(/#([^&#]*)/);
    if (m)
      localSetup.anchor = m[1];
    else
      localSetup.anchor = '';
  }
}

function setupDefaultUrl(url) {
  let m;

  m = url.match(/page=([^&#]*)/);
  if (m)
    localSetup.page = m[1].replace(/.md$/, '');
  else
    localSetup.page = 'index';

  m = url.match(/book=([^&#]*)/);
  if (m)
    localSetup.book = m[1];
  else if (!localSetup.book)
    localSetup.book = 'guide';

  // Extract tab options
  if (!localSetup.tabs)
    localSetup.tabs = {};
  const tabRegex = /[?&](tab-[^=]+)=([^&#]+)/g;
  while ((m = tabRegex.exec(url)) !== null)
    localSetup.tabs[m[1]] = m[2];

  m = url.match(/#([^&#]*)/);
  if (m)
    localSetup.anchor = m[1];
  else
    localSetup.anchor = '';
}

function setupUrl(url) {
  if (isCyberboticsUrl)
    setupCyberboticsUrl(url);
  else
    setupDefaultUrl(url);

  let tabsQuery = '';
  for (let option in localSetup.tabs) {
    if (!localSetup.tabs[option])
      continue;
    if (tabsQuery)
      tabsQuery += ',';
    tabsQuery += option + '=' + localSetup.tabs[option];
  }
  tabsQuery = '[' + tabsQuery + ']';
  console.log('book=' + localSetup.book + ' page=' + localSetup.page + ' branch=' + localSetup.branch + ' tabs=' + tabsQuery + ' anchor=' + localSetup.anchor);
}

function computeTargetPath() {
  let branch = 'released';
  let targetPath = '';
  if (localSetup.branch)
    branch = localSetup.branch;
  if (localSetup.url.startsWith('http'))
    targetPath = localSetup.url + branch + '/docs/';

  targetPath += localSetup.book + '/';

  return targetPath;
}

function redirectUrls(node) {
  // redirect a's href
  const as = node.querySelectorAll('a');
  for (let i = 0; i < as.length; i++) {
    const a = as[i];
    const href = a.getAttribute('href');
    if (!href)
      continue;
    else if (href.startsWith('#'))
      addDynamicAnchorEvent(a); // on firefox, the second click on the anchor is not dealt cleanly
    else if (href.startsWith('http')) // open external links in a new window
      a.setAttribute('target', '_blank');
    else if (href.endsWith('.md') || href.indexOf('.md#') > -1) {
      let match, newPage, anchor;
      if (href.startsWith('../')) { // Cross-book hyperlink case.
        match = /^..\/([\w-]+)\/([\w-]+).md(#[\w-]+)?$/.exec(href);
        if (match && match.length >= 3) {
          const book = match[1];
          newPage = match[2];
          anchor = match[3];
          if (anchor)
            anchor = anchor.substring(1); // remove the '#' character
          a.setAttribute('href', forgeUrl(book, newPage, localSetup.tabs, anchor));
        }
      } else { // Cross-page hyperlink case.
        addDynamicLoadEvent(a);
        match = /^([\w-]+).md(#[\w-]+)?$/.exec(href);
        if (match && match.length >= 2) {
          newPage = match[1];
          anchor = match[2];
          if (anchor)
            anchor = anchor.substring(1); // remove the '#' character
          a.setAttribute('href', forgeUrl(localSetup.book, newPage, localSetup.tabs, anchor));
        }
      }
    }
  }
}

function collapseMovies(node) {
  if (location.href.startsWith('file:')) { // if it's the offline documentation embedded in Webots (possibly without network):
    const iframes = node.querySelectorAll('iframe');
    for (let i = 0; i < iframes.length; i++) { // foreach iframe:
      const iframe = iframes[i];
      let src = iframe.getAttribute('src');
      if (src && src.indexOf('youtube')) { // if the iframe is a youtube frame:
        // then, replace the iframe by a text and an hyperlink to the youtube page.
        src = src.replace(/embed\/(.*)\?rel=0/, 'watch?v=$1'); // e.g. https://www.youtube.com/embed/vFwNwT8dZTU?rel=0 to https://www.youtube.com/watch?v=vFwNwT8dZTU
        let p = document.createElement('p');
        p.innerHTML = '<a href="' + src + '">Click here to see the youtube movie.</a>';
        iframe.parentNode.replaceChild(p, iframe);
      }
    }
  }
}

function forgeUrl(book, page, tabs, anchor) {
  let tabOption;
  let isFirstArgument;
  const anchorString = (anchor && anchor.length > 0) ? ('#' + anchor) : '';
  let url = location.href;
  if (isCyberboticsUrl) {
    const i = location.href.indexOf('cyberbotics.com/doc');
    url = location.href.substr(0, i) + 'cyberbotics.com/doc/' + book + '/' + page;
    if (localSetup.branch !== '' && localSetup.repository && localSetup.repository !== 'cyberbotics')
      url += '?version=' + localSetup.repository + ':' + localSetup.branch;
    else if (localSetup.branch !== '')
      url += '?version=' + localSetup.branch;
    isFirstArgument = localSetup.branch === '';
    for (tabOption in tabs) {
      if (!tabs[tabOption])
        continue;
      url += (isFirstArgument ? '?' : '&') + tabOption + '=' + tabs[tabOption];
      isFirstArgument = false;
    }
    url += anchorString;
  } else {
    isFirstArgument = (url.indexOf('?') < 0);

    // Remove anchor from url
    url = url.split('#')[0];

    // Add or replace the book argument.
    if (url.indexOf('book=') > -1)
      url = url.replace(/book=([^&]+)?/, 'book=' + book);
    else
      url += (isFirstArgument ? '?' : '&') + 'book=' + book;

    // Add or replace the page argument.
    if (url.indexOf('page=') > -1)
      url = url.replace(/page=([\w-]+)?/, 'page=' + page);
    else
      url += '&page=' + page;

    // Add or replace the tab argument.
    for (tabOption in tabs) {
      let tabName = tabs[tabOption] ? tabs[tabOption] : '';
      if (url.indexOf(tabOption + '=') > -1)
        url = url.replace(new RegExp(tabOption + '=([^&]+)(#[\\w-]+)?'), tabOption + '=' + tabName);
      else if (tabName)
        url += '&' + tabOption + '=' + tabName;
    }

    url += anchorString;
  }
  return url;
}

function addDynamicAnchorEvent(el) {
  if (el.classList.contains('dynamicAnchor'))
    return;
  el.addEventListener('click',
    function(event) {
      if (event.ctrlKey)
        return;
      let node = event.target;
      while (node && !node.hasAttribute('href'))
        node = node.getParent();
      if (node) {
        localSetup.anchor = extractAnchor(node.getAttribute('href'));
        applyAnchor();
        event.preventDefault();
      }
    },
    false
  );
  el.classList.add('dynamicAnchor');
}

function addDynamicLoadEvent(el) {
  if (el.classList.contains('dynamicLoad'))
    return;
  el.addEventListener('click',
    function(event) {
      if (event.ctrlKey)
        return;
      aClick(event.target);
      event.preventDefault();
    },
    false
  );
  el.classList.add('dynamicLoad');
}

function aClick(el) {
  setupUrl(el.getAttribute('href'));
  getMDFile();
  updateBrowserUrl();
  updateContributionBannerUrl();
}

function redirectImages(node) {
  // redirect img's src
  const imgs = node.querySelectorAll('img');
  const targetPath = computeTargetPath();
  for (let i = 0; i < imgs.length; i++) {
    const img = imgs[i];
    const src = img.getAttribute('src');
    const match = /^images\/(.*)$/.exec(src);
    if (match && match.length === 2)
      img.setAttribute('src', targetPath + 'images/' + match[1]);
  }
}

function setupModalWindow() {
  const doc = document.querySelector('#webots-doc');

  // Create the following HTML tags:
  // <div id="modal-window" class="modal-window">
  //   <span class="modal-window-close-button">&times;</span>
  //   <img class="modal-window-image-content" />
  //   <div class="modal-window-caption"></div>
  // </div>

  const close = document.createElement('span');
  close.classList.add('modal-window-close-button');
  close.innerHTML = '&times;';
  close.onclick = function() {
    modal.style.display = 'none';
  };

  const loadImage = document.createElement('img');
  loadImage.classList.add('modal-window-load-image');
  loadImage.setAttribute('src', computeTargetPath() + '../css/images/load_animation.gif');

  const image = document.createElement('img');
  image.classList.add('modal-window-image-content');

  const caption = document.createElement('div');
  caption.classList.add('modal-window-caption');

  const modal = document.createElement('div');
  modal.setAttribute('id', 'modal-window');
  modal.classList.add('modal-window');

  modal.appendChild(close);
  modal.appendChild(loadImage);
  modal.appendChild(image);
  modal.appendChild(caption);
  doc.appendChild(modal);

  window.onclick = function(event) {
    if (event.target === modal) {
      modal.style.display = 'none';
      loadImage.style.display = 'block';
      image.style.display = 'none';
    }
  };
}

function updateModalEvents(view) {
  const modal = document.querySelector('#modal-window');
  const image = modal.querySelector('.modal-window-image-content');
  const loadImage = modal.querySelector('.modal-window-load-image');
  const caption = modal.querySelector('.modal-window-caption');

  // Add the modal events on each image.
  const imgs = view.querySelectorAll('img');
  for (let i = 0; i < imgs.length; i++) {
    imgs[i].onclick = function(event) {
      const img = event.target;
      // The modal window is only enabled on big enough images and on thumbnail.
      if (img.src.indexOf('thumbnail') === -1 && !(img.naturalWidth > 128 && img.naturalHeight > 128))
        return;

      // Show the modal window and the caption.
      modal.style.display = 'block';
      caption.innerHTML = (typeof this.parentNode.childNodes[1] !== 'undefined') ? this.parentNode.childNodes[1].innerHTML : '';

      if (img.src.indexOf('.thumbnail.') === -1) {
        // this is not a thumbnail => show the image directly.
        image.src = img.src;
        loadImage.style.display = 'none';
        image.style.display = 'block';
      } else {
        // this is a thumbnail => load the actual image.
        let url = img.src.replace('.thumbnail.', '.');
        if (image.src === url) {
          // The image has already been loaded.
          loadImage.style.display = 'none';
          image.style.display = 'block';
          return;
        } else {
          // The image has to be loaded: show the loading image.
          loadImage.style.display = 'block';
          image.style.display = 'none';
        }
        // In case of thumbnail, search for the original png or jpg
        image.onload = function() {
          // The original image has been loaded successfully => show it.
          loadImage.style.display = 'none';
          image.style.display = 'block';
        };
        image.onerror = function() {
          // The original image has not been loaded successfully => try to change the extension and reload it.
          image.onerror = function() {
            // The original image has not been loaded successfully => abort.
            modal.style.display = 'none';
            loadImage.style.display = 'block';
            image.style.display = 'none';
          };
          url = img.src.replace('.thumbnail.jpg', '.png');
          image.src = url;
        };
        image.src = url;
      }
    };
  }
}

function applyAnchor() {
  const firstAnchor = document.querySelector("[name='" + localSetup.anchor + "']");
  if (firstAnchor) {
    firstAnchor.scrollIntoView(true);
    if (document.querySelector('.contribution-banner'))
      window.scrollBy(0, -38); // GitHub banner.
    if (isCyberboticsUrl)
      window.scrollBy(0, -44); // Cyberbotics header.
  } else
    window.scrollTo(0, 0);
}

function applyToTitleDiv() {
  const titleContentElement = document.querySelector('#title-content');
  if (titleContentElement) {
    let newTitle;
    if (localSetup.book === 'guide')
      newTitle = 'Webots User Guide';
    else if (localSetup.book === 'reference')
      newTitle = 'Webots Reference Manual';
    else if (localSetup.book === 'blog')
      newTitle = 'Webots Blog';
    else if (localSetup.book === 'discord')
      newTitle = 'Webots Discord Archives';
    else if (localSetup.book === 'automobile')
      newTitle = 'Webots for automobiles';
    else
      newTitle = '';
    if (newTitle.length > 0) {
      newTitle += " <div class='release-tag'>" + getWebotsVersion() + '</div>';
      titleContentElement.innerHTML = newTitle;
    }
  }
}

function addContributionBanner() {
  // if we're on the website we need to move the banner down by the height of the navbar
  const displacement = isCyberboticsUrl ? '44px' : '0px';

  // append contribution sticker to primary doc element
  document.querySelector('#center').innerHTML += '<div style="top:' + displacement + '" class="contribution-banner">' +
                                                 'Found an error?' +
                                                 '<a target="_blank" class="contribution-banner-url" href="https://github.com/cyberbotics/webots/tree/released/docs"> ' +
                                                 'Contribute on GitHub!' +
                                                 '<span class=github-logo />' +
                                                 '</a>' +
                                                 '<p id="contribution-close">X</p>' +
                                                 '</div>';
  updateContributionBannerUrl();

  const contributionBanner = document.querySelector('.contribution-banner');

  document.querySelector('#contribution-close').onclick = function() {
    contributionBanner.parentNode.removeChild(contributionBanner);
  };
}

function updateContributionBannerUrl() {
  const contributionBanner = document.querySelector('.contribution-banner-url');
  if (contributionBanner)
    contributionBanner.href = 'https://github.com/cyberbotics/webots/edit/released/docs/' + localSetup.book + '/' + localSetup.page + '.md';
}

function addNavigationToBlogIfNeeded() {
  if (!document.querySelector('#next-previous-section') && localSetup.book === 'blog') {
    let menu = document.querySelector('#menu');
    let lis = menu.querySelectorAll('li');
    let currentPageIndex = -1;
    for (let i = 0; i < lis.length; ++i) {
      if (lis[i].className === 'selected') {
        currentPageIndex = i;
        break;
      }
    }

    if (currentPageIndex === -1)
      return;

    // console.log(currentPageIndex, lis.length);
    // console.log(lis);
    let div = document.createElement('div');
    div.setAttribute('id', 'next-previous-section');
    // previous post
    if (currentPageIndex > 0) {
      let previous = lis[currentPageIndex - 1];
      let a = previous.firstChild.cloneNode();
      a.innerHTML += '<< Previous Post: ' + previous.textContent;
      a.setAttribute('class', 'post-selector left');
      div.appendChild(a);
    }

    if (currentPageIndex < lis.length - 1) {
      let next = lis[currentPageIndex + 1];
      let a = next.firstChild.cloneNode();
      a.innerHTML += 'Next Post: ' + next.textContent + ' >>';
      a.setAttribute('class', 'post-selector right');
      div.appendChild(a);
    }

    document.querySelector('#publish-data').parentNode.insertBefore(div, document.querySelector('#publish-data').nextSibling);
  }
}

function setupBlogFunctionalitiesIfNeeded() {
  if (localSetup.book === 'blog') {
    // hide index, this doesn't make sense for a blog post
    let index = document.querySelector('#index');
    let indexTitle = document.querySelector('#indexTitle');

    if (index !== null)
      index.style.display = 'none';

    if (indexTitle !== null)
      indexTitle.style.display = 'none';

    // hide the release tag, this is also nonsensical here
    document.querySelector('.release-tag').style.display = 'none';

    document.title = document.title.replace('documentation', 'Blog');
  }
}

function createIndex(view) {
  // Note: the previous index is cleaned up when the parent title is destroyed.

  // Get all the view headings.
  const headings = [].slice.call(view.querySelectorAll('h1, h2, h3, h4'));

  // Do not create too small indexes.
  const content = document.querySelector('#content');
  if ((content.offsetHeight < 2 * window.innerHeight || headings.length < 4) && (localSetup.book !== 'discord' || headings.length < 2))
    return;

  let level = parseInt(headings[0].tagName[1]) + 1; // current heading level.

  // Create an empty index, and insert it before the second heading.
  const indexTitle = document.createElement('h' + level);
  indexTitle.textContent = 'Index';
  indexTitle.setAttribute('id', 'indexTitle');
  headings[0].parentNode.insertBefore(indexTitle, headings[1]);
  let ul = document.createElement('ul');
  ul.setAttribute('id', 'index');
  headings[0].parentNode.insertBefore(ul, headings[1]);

  headings.forEach(function(heading, i) {
    if (i === 0) // Skip the first heading.
      return;

    // Update current level and ul.
    const newLevel = parseInt(heading.tagName[1]);
    while (newLevel > level) {
      const newUl = document.createElement('ul');
      ul.appendChild(newUl);
      ul = newUl;
      level += 1;
    }
    while (newLevel < level) {
      ul = ul.parentNode;
      level -= 1;
    }

    // Add the <li> tag.
    const anchor = heading.getAttribute('name');
    const a = document.createElement('a');
    a.setAttribute('href', '#' + anchor);
    a.textContent = heading.textContent;
    const li = document.createElement('li');
    li.appendChild(a);
    ul.appendChild(li);
  });
}

function getWebotsVersion() {
  if (localSetup.branch)
    return localSetup.branch;
  // Get the Webots version from the showdown wbVariables extension
  const version = '{{ webots.version.full }}';
  const converter = new showdown.Converter({extensions: ['wbVariables']});
  const html = converter.makeHtml(version);
  const tmp = document.createElement('div');
  tmp.innerHTML = html;
  return tmp.textContent || tmp.innerText || '';
}

function applyToPageTitle(mdContent) {
  let hashtagIndex = mdContent.indexOf('#');
  if (hashtagIndex >= 0) {
    while (hashtagIndex + 1 < mdContent.length && mdContent[hashtagIndex + 1] === '#')
      hashtagIndex += 1;
    const hashtagCarriageReturn = mdContent.indexOf('\n', hashtagIndex);
    if (hashtagCarriageReturn >= 0) {
      const title = mdContent.substring(hashtagIndex + 1, hashtagCarriageReturn).trim();
      document.title = 'Webots documentation: ' + title;
    }
  }
}

function populateViewDiv(mdContent) {
  setupUrl(document.location.href);

  const view = document.querySelector('#view');
  while (view.firstChild)
    view.removeChild(view.firstChild);

  // console.log('Raw MD content:\n\n');
  // console.log(mdContent);

  applyToPageTitle(mdContent);

  // markdown to html
  window.mermaidGraphCounter = 0;
  window.mermaidGraphs = {};
  const converter = new showdown.Converter({tables: 'True', extensions: ['wbTabComponent', 'wbRobotComponent', 'wbSpoiler', 'wbChart', 'wbVariables', 'wbAPI', 'wbFigure', 'wbAnchors', 'wbIllustratedSection', 'youtube']});
  const html = converter.makeHtml(mdContent);

  // console.log('HTML content: \n\n')
  // console.log(html);

  view.innerHTML = html;

  createRobotComponent(view);
  renderGraphs();
  redirectImages(view);
  updateModalEvents(view);
  redirectUrls(view);
  collapseMovies(view);

  applyAnchorIcons(view);
  highlightCode(view);

  updateSelection();
  createIndex(view);

  setupBlogFunctionalitiesIfNeeded();
  addNavigationToBlogIfNeeded();

  const images = view.querySelectorAll('img');
  if (images.length > 0) {
    // apply the anchor only when the images are loaded,
    // otherwise, the anchor can be overestimated.
    const lastImage = images[images.length - 1];
    lastImage.onload = () => applyAnchor();
  } else
    applyAnchor();
  applyTabs();
}

// replace the browser URL after a dynamic load
function updateBrowserUrl() {
  const url = forgeUrl(localSetup.book, localSetup.page, localSetup.tabs, localSetup.anchor);
  if (history.pushState) {
    try {
      history.pushState({state: 'new'}, null, url);
    } catch (err) {
    }
  }
  const canonicalUrl = 'https://cyberbotics.com/doc/' + localSetup.book + '/' + localSetup.page;
  const canonical = document.querySelector('link[rel="canonical"]');
  if (canonical !== null)
    canonical.href = canonicalUrl;
}

// Make in order that the back button is working correctly
window.onpopstate = function(event) {
  setupUrl(document.location.href);
  getMDFile();
};

function highlightCode(view) {
  const supportedLanguages = ['c', 'cpp', 'java', 'python', 'matlab', 'sh', 'ini', 'tex', 'makefile', 'lua', 'xml'];

  for (let i = 0; i < supportedLanguages.length; i++) {
    const language = supportedLanguages[i];
    hljs.configure({languages: [ language ]});
    const codes = document.querySelectorAll('.' + language);
    for (let j = 0; j < codes.length; j++) {
      const code = codes[j];
      hljs.highlightBlock(code);
    }
  }
}

function resetRobotComponent(robot) {
  unhighlightX3DElement(robot);
  const robotComponent = getRobotComponentByRobotName(robot);
  // Reset the Viewpoint
  if (typeof WbWorld.instance !== 'undefined' && typeof WbWorld.instance.viewpoint !== 'undefined')
    WbWorld.instance.viewpoint.resetViewpoint();
  // Reset the motor sliders.
  let sliders = robotComponent.querySelectorAll('.motor-slider');
  for (let s = 0; s < sliders.length; s++) {
    let slider = sliders[s];
    slider.value = slider.getAttribute('webots-position');
    let id = slider.getAttribute('webots-transform-id');
    sliderMotorCallback(WbWorld.instance.nodes.get(id), slider);
  }
  robotComponent.webotsView.x3dScene.render();
}

function updateRobotComponentDimension(robot) {
  const robotComponent = getRobotComponentByRobotName(robot);
  if (typeof robotComponent === 'undefined')
    return;
  const deviceMenu = robotComponent.querySelector('.device-component');
  const robotView = robotComponent.querySelector('.robot-view');

  if (typeof robotComponent.showDeviceComponent === 'undefined')
    robotComponent.showDeviceComponent = true;
  if (robotComponent.showDeviceComponent === true) {
    deviceMenu.style.display = '';
    robotView.style.width = '70%';
  } else {
    deviceMenu.style.display = 'none';
    robotView.style.width = '100%';
  }

  robotComponent.webotsView.x3dScene.resize();
}

function toggleDeviceComponent(robot) {
  const robotComponent = getRobotComponentByRobotName(robot);
  if (typeof robotComponent.showDeviceComponent === 'undefined')
    robotComponent.showDeviceComponent = true;
  robotComponent.showDeviceComponent = !robotComponent.showDeviceComponent;
  updateRobotComponentDimension(robot);

  let arrow = document.getElementById('arrow');
  if (arrow) {
    if (arrow.className === 'arrow-right')
      arrow.className = 'arrow-left';
    else if (arrow.className === 'arrow-left')
      arrow.className = 'arrow-right';
  }
}

function toggleRobotComponentFullScreen(robot) { // eslint-disable-line no-unused-lets
  // Source: https://stackoverflow.com/questions/7130397/how-do-i-make-a-div-full-screen
  let element = getRobotComponentByRobotName(robot);
  if (
    document.fullscreenElement ||
    document.webkitFullscreenElement ||
    document.mozFullScreenElement ||
    document.msFullscreenElement
  ) {
    document.getElementsByClassName('fullscreen-button')[0].style.display = '';
    document.getElementsByClassName('exit-fullscreen-button')[0].style.display = 'none';

    if (document.exitFullscreen)
      document.exitFullscreen();
    else if (document.mozCancelFullScreen)
      document.mozCancelFullScreen();
    else if (document.webkitExitFullscreen)
      document.webkitExitFullscreen();
    else if (document.msExitFullscreen)
      document.msExitFullscreen();
  } else {
    document.getElementsByClassName('fullscreen-button')[0].style.display = 'none';
    document.getElementsByClassName('exit-fullscreen-button')[0].style.display = '';

    if (element.requestFullscreen) {
      element.requestFullscreen();
      document.addEventListener('fullscreenchange', function() {
        updateRobotComponentDimension(robot);
      });
    } else if (element.mozRequestFullScreen) {
      element.mozRequestFullScreen();
      document.addEventListener('mozfullscreenchange', function() {
        updateRobotComponentDimension(robot);
      });
    } else if (element.webkitRequestFullscreen) {
      element.webkitRequestFullscreen(Element.ALLOW_KEYBOARD_INPUT);
      document.addEventListener('webkitfullscreenchange', function() {
        updateRobotComponentDimension(robot);
      });
    } else if (element.msRequestFullscreen) {
      element.msRequestFullscreen();
      document.addEventListener('msfullscreenchange', function() {
        updateRobotComponentDimension(robot);
      });
    }
  }
}

function sliderMotorCallback(transform, slider) {
  if (typeof transform === 'undefined')
    return;

  if (typeof transform.firstRotation === 'undefined' && typeof transform.rotation !== 'undefined')
    transform.firstRotation = transform.rotation.clone();

  if (typeof transform.firstPosition === 'undefined' && typeof transform.translation !== 'undefined')
    transform.firstPosition = transform.translation.clone();

  let axis = slider.getAttribute('webots-axis').split(/[\s,]+/);
  axis = glm.vec3(parseFloat(axis[0]), parseFloat(axis[1]), parseFloat(axis[2]));

  let value = parseFloat(slider.value);
  let position = parseFloat(slider.getAttribute('webots-position'));

  if (slider.getAttribute('webots-type') === 'LinearMotor') {
    // Compute translation
    let translation = new WbVector3();
    if (typeof transform.initialTranslation !== 'undefined')
      translation = transform.initialTranslation.clone();
    else {
      translation = transform.translation;
      transform.initialTranslation = translation.clone();
    }
    translation = translation.add(axis.mul(value - position));
    // Apply the new position.
    transform.translation = translation;
    transform.applyTranslationToWren();
  } else {
    // extract anchor
    let anchor = slider.getAttribute('webots-anchor').split(/[\s,]+/);
    anchor = new WbVector3(parseFloat(anchor[0]), parseFloat(anchor[1]), parseFloat(anchor[2]));

    // Compute angle.
    let angle = value - position;

    // Apply the new axis-angle.
    let q = glm.angleAxis(angle, axis);

    if (typeof transform.firstRotation !== 'undefined')
      q = q.mul(vec4ToQuaternion(transform.firstRotation));

    if (typeof transform.firstPosition !== 'undefined')
      transform.translation = transform.firstPosition.clone();

    transform.translation = transform.translation.sub(anchor); // remove the offset

    let quat = glm.angleAxis(angle, axis); // rotate the POSITION

    transform.translation = applyQuaternion(transform.translation, quat);
    transform.translation = transform.translation.add(anchor); // re-add the offset
    transform.rotation = quaternionToVec4(q);
    transform.applyTranslationToWren();
    transform.applyRotationToWren();
  }
}

function applyQuaternion(vec3, q) {
  const x = vec3.x;
  const y = vec3.y;
  const z = vec3.z;
  const qx = q.x;
  const qy = q.y;
  const qz = q.z;
  const qw = q.w;

  // calculate quat * vector

  const ix = qw * x + qy * z - qz * y;
  const iy = qw * y + qz * x - qx * z;
  const iz = qw * z + qx * y - qy * x;
  const iw = -qx * x - qy * y - qz * z;

  // calculate result * inverse quat

  vec3.x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
  vec3.y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
  vec3.z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

  return vec3;
}

function removePointer() {
  if (typeof pointer !== 'undefined') {
    pointer.delete();
    pointer = undefined;
  }
}

function unhighlightX3DElement(robot) {
  const robotComponent = getRobotComponentByRobotName(robot);
  const scene = robotComponent.webotsView.x3dScene;
  removePointer();
  scene.render();
}

function highlightX3DElement(robot, deviceElement) {
  if (typeof imageTexture === 'undefined') {
    imageTexture = new WbImageTexture(getAnId(), undefined, computeTargetPath() + '../css/images/marker.png', false, true, true, 4);
    imageTexture.updateUrl().then(() => {
      // highlight again when the texture is loaded
      highlightX3DElement(robot, deviceElement);
    });
  }
  unhighlightX3DElement(robot);

  let robotComponent = getRobotComponentByRobotName(robot);
  let scene = robotComponent.webotsView.x3dScene;
  let id = deviceElement.getAttribute('webots-transform-id');
  if (typeof WbWorld.instance === 'undefined')
    return;
  let object = WbWorld.instance.nodes.get(id);
  if (object) {
    if (typeof WbWorld.instance !== 'undefined' && typeof pointer === 'undefined') {
      if (typeof sizeOfMarker === 'undefined') {
        // We estimate the size of the robot by the calculating the distance between the robot and the viewpoint
        let robotPosition = WbWorld.instance.sceneTree[WbWorld.instance.sceneTree.length - 1].translation;
        let viewpointPosition = WbWorld.instance.viewpoint.position;
        sizeOfMarker = 0.012 * robotPosition.sub(viewpointPosition).length(); // value determined empirically
      }
      let sphere = new WbSphere(getAnId(), sizeOfMarker, true, 5);
      WbWorld.instance.nodes.set(sphere.id, sphere);
      let baseColorMap = imageTexture.clone(getAnId());
      baseColorMap.type = 'baseColorMap';
      WbWorld.instance.nodes.set(baseColorMap.id, baseColorMap);
      let pbr = new WbPBRAppearance(getAnId(), new WbVector3(1, 1, 1), baseColorMap, 0, 1, undefined, 0, undefined,
        20, undefined, 1, undefined, 1, new WbVector3(0, 0, 0), undefined, 1, undefined);
      baseColorMap.parent = pbr.id;
      WbWorld.instance.nodes.set(pbr.id, pbr);
      let shape = new WbShape(getAnId(), false, false, sphere, pbr);
      WbWorld.instance.nodes.set(shape.id, shape);
      sphere.parent = shape.id;
      pbr.parent = shape.id;
      pointer = new WbTransform(getAnId(), false, new WbVector3(0, 0, 0), new WbVector3(1, 1, 1), new WbVector4());
      pointer.children.push(shape);
      WbWorld.instance.nodes.set(pointer.id, pointer);
      shape.parent = pointer.id;
    }

    if (deviceElement.hasAttribute('webots-transform-offset')) {
      let offset = deviceElement.getAttribute('webots-transform-offset').split(/[\s,]+/);
      pointer.translation = new WbVector3(parseFloat(offset[0]), parseFloat(offset[1]), parseFloat(offset[2]));
    }

    object.children.push(pointer);
    pointer.parent = object.id;
    pointer.children[0].geometry.isMarker = true;
    pointer.finalize();

    scene.render();
  }
}

function getRobotComponentByRobotName(robotName) {
  return document.querySelector('#' + robotName + '-robot-component');
}

function createRobotComponent(view) {
  const robotComponents = document.querySelectorAll('.robot-component');
  for (let c = 0; c < robotComponents.length; c++) { // foreach robot components of this page.
    const robotComponent = robotComponents[c];
    const webotsViewElement = document.querySelectorAll('.robot-webots-view')[0];
    const robotName = webotsViewElement.getAttribute('id').replace('-robot-webots-view', '');

    if (typeof webotsView === 'undefined')
      webotsView = new webots.View(webotsViewElement);
    else {
      webotsView.x3dScene.destroyWorld();
      sizeOfMarker = undefined;
      pointer = undefined;
      webotsView.view3D = webotsViewElement;
    }
    robotComponent.webotsView = webotsView; // Store the Webots view in the DOM element for a simpler access.

    // Load the robot X3D file.
    webotsView.open(
      computeTargetPath() + 'scenes/' + robotName + '/' + robotName + '.x3d',
      undefined,
      computeTargetPath() + 'scenes/' + robotName + '/'
    );

    // Load the robot meta JSON file.
    fetch(computeTargetPath() + 'scenes/' + robotName + '/' + robotName + '.meta.json')
      .then(response => response.json())
      .then(function(data) {
        // Populate the device component from the JSON file.
        let deviceComponent = view.querySelector('#' + robotName + '-device-component');
        let categories = {};
        robotComponent.setAttribute('robot-node-id', data['robotID']);
        if (data['devices'].length === 0)
          toggleDeviceComponent(robotName);
        for (let d = 0; d < data['devices'].length; d++) {
          const device = data['devices'][d];
          const deviceName = device['name'];
          const deviceType = device['type'];

          // Create or retrieve the device category container.
          let category = null;
          if (deviceType in categories)
            category = categories[deviceType];
          else {
            category = document.createElement('div');
            category.classList.add('device-category');
            category.innerHTML = '<div class="device-title">' + deviceType + '</div>';
            deviceComponent.appendChild(category);
            categories[deviceType] = category;
          }

          // Create the new device.
          let deviceDiv = document.createElement('div');
          deviceDiv.classList.add('device');
          deviceDiv.addEventListener('mouseover', () => highlightX3DElement(robotName, deviceDiv));
          deviceDiv.setAttribute('webots-type', deviceType);
          deviceDiv.setAttribute('webots-transform-id', device['transformID']);
          if ('transformOffset' in device) // The Device Transform has not been exported. The device is defined relatively to it's Transform parent.
            deviceDiv.setAttribute('webots-transform-offset', device['transformOffset']);
          deviceDiv.innerHTML = '<div class="device-name">' + deviceName + '</div>';

          // Create the new motor.
          if (deviceType.endsWith('Motor') && !device['track']) {
            const minLabel = document.createElement('div');
            minLabel.classList.add('motor-label');
            const maxLabel = document.createElement('div');
            maxLabel.classList.add('motor-label');
            const slider = document.createElement('input');
            slider.classList.add('motor-slider');
            slider.setAttribute('type', 'range');
            slider.setAttribute('step', 'any');
            if (device['minPosition'] === device['maxPosition']) { // infinite range.
              slider.setAttribute('min', -Math.PI);
              slider.setAttribute('max', Math.PI);
              minLabel.innerHTML = -3.14; // 2 decimals.
              maxLabel.innerHTML = 3.14;
            } else { // fixed range.
              const epsilon = 0.000001; // To solve Windows browser bugs on slider when perfectly equals to 0.
              slider.setAttribute('min', device['minPosition'] - epsilon);
              slider.setAttribute('max', device['maxPosition'] + epsilon);
              minLabel.innerHTML = Math.round(device['minPosition'] * 100) / 100; // 2 decimals.
              maxLabel.innerHTML = Math.round(device['maxPosition'] * 100) / 100;
            }
            slider.setAttribute('value', device['position']);
            slider.setAttribute('webots-position', device['position']);
            slider.setAttribute('webots-transform-id', device['transformID']);
            slider.setAttribute('webots-axis', device['axis']);
            slider.setAttribute('webots-anchor', device['anchor']);
            slider.setAttribute('webots-type', deviceType);
            slider.addEventListener(isInternetExplorer() ? 'change' : 'input', _ => {
              if (typeof WbWorld.instance === 'undefined')
                return;
              let id = _.target.getAttribute('webots-transform-id');
              sliderMotorCallback(WbWorld.instance.nodes.get(id), _.target);
              webotsView.x3dScene.render();
            });

            const motorDiv = document.createElement('div');
            motorDiv.classList.add('motor-component');
            motorDiv.appendChild(minLabel);
            motorDiv.appendChild(slider);
            motorDiv.appendChild(maxLabel);
            deviceDiv.appendChild(motorDiv);
            deviceDiv.setAttribute('device-anchor', device['anchor']);
          }

          category.appendChild(deviceDiv);
        }
      })
      .catch(error => {
        console.log('Error: ' + error);
      });

    if (document.getElementsByClassName('menu-button').length !== 0)
      document.getElementsByClassName('menu-button')[0].onclick = () => toggleDeviceComponent(robotName);
    if (document.getElementsByClassName('fullscreen-button').length !== 0)
      document.getElementsByClassName('fullscreen-button')[0].onclick = () => toggleRobotComponentFullScreen(robotName);
    if (document.getElementsByClassName('exit-fullscreen-button').length !== 0) {
      document.getElementsByClassName('exit-fullscreen-button')[0].onclick = () => toggleRobotComponentFullScreen(robotName);
      document.getElementsByClassName('exit-fullscreen-button')[0].style.display = 'none';
    }
    if (document.getElementsByClassName('reset-button').length !== 0)
      document.getElementsByClassName('reset-button')[0].onclick = () => resetRobotComponent(robotName);

    if (document.getElementsByClassName('robot-component').length !== 0) {
      document.getElementsByClassName('robot-component')[0].onmouseenter = () => showButtons();
      document.getElementsByClassName('robot-component')[0].onmouseleave = () => hideButtons(robotName);
    }
  }
}

function showButtons() {
  if (document.getElementsByClassName('reset-button').length !== 0)
    document.getElementsByClassName('reset-button')[0].style.display = '';

  if (document.getElementsByClassName('fullscreen-button').length !== 0)
    document.getElementsByClassName('fullscreen-button')[0].style.display = '';

  if (document.getElementsByClassName('menu-button').length !== 0)
    document.getElementsByClassName('menu-button')[0].style.display = '';
}

function hideButtons(robot) {
  if (document.getElementsByClassName('reset-button').length !== 0)
    document.getElementsByClassName('reset-button')[0].style.display = 'none';

  if (document.getElementsByClassName('fullscreen-button').length !== 0)
    document.getElementsByClassName('fullscreen-button')[0].style.display = 'none';

  if (document.getElementsByClassName('exit-fullscreen-button').length !== 0)
    document.getElementsByClassName('exit-fullscreen-button')[0].style.display = 'none';

  if (document.getElementsByClassName('menu-button').length !== 0)
    document.getElementsByClassName('menu-button')[0].style.display = 'none';

  removePointer();
  const robotComponent = getRobotComponentByRobotName(robot);
  robotComponent.webotsView.x3dScene.render();
}

// Open a tab component tab
function openTabFromEvent(evt, option, name) {
  // update links
  const a = document.querySelectorAll('a');
  for (let i = 0; i < a.length; i++) {
    const href = a[i].getAttribute('href');
    if (!href)
      continue;
    if (localSetup.tabs[option]) {
      if (href.includes(option + '=' + localSetup.tabs[option]))
        a[i].setAttribute('href', href.replace(option + '=' + localSetup.tabs[option], option + '=' + name.toLowerCase()));
      else if (!href.startsWith('#'))
        a[i].setAttribute('href', href + (href.indexOf('?') > -1 ? '&' : '?') + option + '=' + name.toLowerCase());
    }
  }
  // open tab
  localSetup.tabs[option] = name.toLowerCase();
  updateBrowserUrl();
  openTab(evt.target.parentNode, localSetup.tabs[option]);
}

// Open a tab component tab
function openTab(tabcomponent, name) {
  const tabID = tabcomponent.getAttribute('tabid');

  let tabcontent = tabcomponent.parentNode.querySelectorAll('.tab-content[tabid="' + tabID + '"][name="' + name + '"]')[0];
  if (typeof tabcontent === 'undefined')
    return false;

  const tabcontents = tabcomponent.parentNode.querySelectorAll('.tab-content[tabid="' + tabID + '"]');
  for (let i = 0; i < tabcontents.length; i++)
    tabcontents[i].style.display = 'none';

  const tablinks = tabcomponent.querySelectorAll('.tab-links');
  for (let j = 0; j < tablinks.length; j++)
    tablinks[j].classList.remove('active');

  tabcontent = tabcomponent.parentNode.querySelectorAll('.tab-content[tabid="' + tabID + '"][name="' + name + '"]')[0];
  tabcontent.style.display = 'block';

  const tablink = tabcomponent.querySelectorAll('.tab-links[name="' + name + '"]')[0];
  tablink.classList.add('active');
  return true;
}

function applyTabs() {
  const tabLinks = document.getElementsByClassName('tab-links');
  for (let i = 0; i < tabLinks.length; i++) {
    let element = tabLinks[i];
    element.onclick = _ => openTabFromEvent(_, 'tab-' + element.title, element.innerHTML);
  }

  const tabComponents = document.querySelectorAll('.tab-component');
  for (let k = 0; k < tabComponents.length; k++) {
    for (let tabName in localSetup.tabs) {
      if (openTab(tabComponents[k], localSetup.tabs[tabName]))
        break;
    }
  }
}

function renderGraphs() {
  for (let id in window.mermaidGraphs) {
    window.mermaidAPI.render(id, window.mermaidGraphs[id], function(svgCode, bindFunctions) {
      document.querySelector('#' + id + 'Div').innerHTML = svgCode;
      // set min-width to be 2/3 of the max-width otherwise the text might become too small
      const element = document.querySelector('#' + id);
      const style = element.getAttribute('style');
      element.setAttribute('style', style + ' min-width:' + Math.floor(0.66 * parseInt(style.split('max-width:')[1].split('px'))) + 'px;');
    });
  }
}

function applyAnchorIcons(view) {
  let elements = [];
  const tags = ['figcaption', 'h1', 'h2', 'h3', 'h4', 'h5'];
  for (let i = 0; i < tags.length; i++) {
    const array = Array.prototype.slice.call(view.querySelectorAll(tags[i]));
    elements = elements.concat(array);
  }
  for (let i = 0; i < elements.length; i++) {
    const el = elements[i];
    let name = null;
    if (el.parentNode && el.tagName.toLowerCase() === 'figcaption' && el.parentNode.tagName.toLowerCase() === 'figure')
      name = el.parentNode.getAttribute('name');
    else
      name = el.getAttribute('name');
    if (name) {
      el.classList.add('anchor-header');
      const span = document.createElement('span');
      span.classList.add('anchor-link-image');
      const a = document.createElement('a');
      a.setAttribute('href', '#' + name);
      a.classList.add('anchor-link');
      a.appendChild(span);
      el.insertBefore(a, el.firstChild);
    }
  }
}

function receiveMenuContent(menuContent) {
  // console.log('Menu content:\n\n');
  // console.log(menuContent);

  let menu = null;

  const converter = new showdown.Converter();
  const html = converter.makeHtml(menuContent);
  const div = document.createElement('div');
  div.innerHTML = html;

  for (let i = 0; i < div.childNodes.length; i++) {
    const child = div.childNodes[i];
    if (child && child.tagName && child.tagName.length > 0 && child.tagName.toLowerCase() === 'ul') {
      menu = child;
      break;
    }
  }

  if (!menu) {
    console.error('Cannot extract Menu.');
    return;
  }

  populateMenu(menu);
  redirectUrls(menu);
  updateSelection();
  addNavigationToBlogIfNeeded();
}

function updateMenuScrollbar() {
  const e = document.documentElement;
  const t = document.documentElement.scrollTop || document.body.scrollTop;
  const p = e.scrollHeight - t - e.clientHeight;
  const footerHeight = 192;
  if (p < footerHeight)
    document.querySelector('#left').style.height = (e.clientHeight - footerHeight + p) + 'px';
  else
    document.querySelector('#left').style.height = '100%';
}

function updateSelection() {
  const selected = changeMenuSelection();
  populateNavigation(selected);
  if (isCyberboticsUrl)
    updateMenuScrollbar();
}

function changeMenuSelection() {
  const menu = document.querySelector('#menu');
  const selecteds = [].slice.call(menu.querySelectorAll('.selected'));
  let selected;
  for (let i = 0; i < selecteds.length; i++) {
    selected = selecteds[i];
    selected.classList.remove('selected');
  }
  const as = menu.querySelectorAll('a');
  for (let i = 0; i < as.length; i++) {
    const a = as[i];
    let href = a.getAttribute('href');
    let selection;
    if (!isCyberboticsUrl) {
      const pageIndex = href.indexOf('page=' + localSetup.page);
      // Notes:
      // - the string length test is done to avoid wrong positive cases
      //   where a page is a prefix of another.
      // - 5 matches with the 'page=' string length.
      if (pageIndex > -1 && (5 + pageIndex + localSetup.page.length) === href.length)
        selection = true;
      else
        selection = false;
    } else {
      let n = href.indexOf('?');
      if (n > -1)
        href = href.substring(0, n);
      n = href.indexOf('#');
      if (n > -1)
        href = href.substring(0, n);
      if (href.endsWith('/doc/' + localSetup.book + '/' + localSetup.page))
        selection = true;
      else
        selection = false;
    }
    if (selection) {
      selected = a.parentNode;
      selected.classList.add('selected');
      if (selected.parentNode.parentNode.tagName.toLowerCase() === 'li') {
        selected.parentNode.parentNode.classList.add('selected');
        const firstChild = selected.parentNode.parentNode.firstChild;
        if (firstChild.tagName.toLowerCase() === 'a')
          showAccodionItem(firstChild);
      } else
        showAccodionItem(a);
      return selected;
    }
  }
}

function populateNavigation(selected) {
  const next = document.querySelector('#next');
  const previous = document.querySelector('#previous');
  const up = document.querySelector('#up');
  const toc = document.querySelector('#toc');
  let as;

  toc.setAttribute('href', forgeUrl(localSetup.book, 'menu'));
  addDynamicLoadEvent(toc);

  if (!selected) {
    next.classList.add('disabled');
    previous.classList.add('disabled');
    up.classList.add('disabled');
    return;
  }

  if (next) {
    let nextElement = null;

    let nextLiSibling = selected.nextSibling;
    while (nextLiSibling) {
      if (nextLiSibling.tagName && nextLiSibling.tagName.toLowerCase() === 'li')
        break;
      nextLiSibling = nextLiSibling.nextSibling;
    }
    if (nextLiSibling) {
      as = nextLiSibling.querySelectorAll('a');
      if (as.length > 0)
        nextElement = as[0];
    }

    if (nextElement) {
      next.classList.remove('disabled');
      next.setAttribute('href', nextElement.getAttribute('href'));
      addDynamicLoadEvent(next);
    } else
      next.classList.add('disabled');
  }

  if (previous) {
    let previousElement = null;

    let previousLiSibling = selected.previousSibling;
    while (previousLiSibling) {
      if (previousLiSibling.tagName && previousLiSibling.tagName.toLowerCase() === 'li')
        break;
      previousLiSibling = previousLiSibling.previousSibling;
    }
    if (previousLiSibling) {
      as = previousLiSibling.querySelectorAll('a');
      if (as.length > 0)
        previousElement = as[0];
    }

    if (previousElement) {
      previous.classList.remove('disabled');
      previous.setAttribute('href', previousElement.getAttribute('href'));
      addDynamicLoadEvent(previous);
    } else
      previous.classList.add('disabled');
  }

  if (up) {
    let upElement = null;
    let parentLi = null;
    if (selected.parentNode.parentNode.tagName.toLowerCase() === 'li')
      parentLi = selected.parentNode.parentNode;
    if (parentLi) {
      as = parentLi.querySelectorAll('a');
      if (as.length > 0)
        upElement = as[0];
    }

    if (upElement) {
      up.classList.remove('disabled');
      up.setAttribute('href', upElement.getAttribute('href'));
      addDynamicLoadEvent(up);
    } else {
      up.setAttribute('href', forgeUrl(localSetup.book, 'index'));
      addDynamicLoadEvent(up);
      up.classList.remove('disabled');
    }
  }
}

function populateMenu(menu) {
  // make in order that the <li> tags above the <a> are also clickable
  const lis = menu.querySelectorAll('li');
  for (let i = 0; i < lis.length; i++) {
    const li = lis[i];
    li.addEventListener('click',
      function(event) {
        const as = event.target.querySelectorAll('a');
        if (as.length > 0)
          aClick(as[0]);
      }
    );
  }

  const menuDiv = document.querySelector('#menu');
  menuDiv.appendChild(menu);

  menu.setAttribute('id', 'accordion');
  let list = document.querySelectorAll('#accordion > li > a');
  list.forEach(item => {
    item.onclick = () => showAccodionItem(item);
  });
}

function showAccodionItem(item) {
  if (!item.className.includes('active')) {
    document.querySelectorAll('#accordion li ul').forEach(ul => {
      if (item.nextElementSibling !== ul)
        slideUp(ul);
    });

    slideToggle(item.nextElementSibling);
    document.querySelectorAll('#accordion li a').forEach(item => {
      item.classList.remove('active');
    });
    item.className += ' active';
  }
}

// Taken from https://dev.to/bmsvieira/vanilla-js-slidedown-up-4dkn
function slideUp(item) {
  if (!item)
    return;

  item.style.transitionProperty = 'height, margin, padding';
  item.style.transitionDuration = 500 + 'ms';
  item.style.boxSizing = 'border-box';
  item.style.height = item.offsetHeight + 'px';
  item.offsetHeight; // Do not remove, the transition is not correctly executed otherwise
  item.style.overflow = 'hidden';
  item.style.height = 0;
  item.style.paddingTop = 0;
  item.style.paddingBottom = 0;
  item.style.marginTop = 0;
  item.style.marginBottom = 0;
  window.setTimeout(() => {
    item.style.display = 'none';
    item.style.removeProperty('height');
    item.style.removeProperty('padding-top');
    item.style.removeProperty('padding-bottom');
    item.style.removeProperty('margin-top');
    item.style.removeProperty('margin-bottom');
    item.style.removeProperty('overflow');
    item.style.removeProperty('transition-duration');
    item.style.removeProperty('transition-property');
  }, 500);
}

function slideToggle(item) {
  if (!item)
    return;

  item.style.removeProperty('display');
  let display = window.getComputedStyle(item).display;
  if (display === 'none') display = 'block';
  item.style.display = display;
  let height = item.offsetHeight;
  item.style.overflow = 'hidden';
  item.style.height = 0;
  item.style.paddingTop = 0;
  item.style.paddingBottom = 0;
  item.style.marginTop = 0;
  item.style.marginBottom = 0;
  item.offsetHeight; // Do not remove, the transition is not correctly executed otherwise
  item.style.boxSizing = 'border-box';
  item.style.transitionProperty = 'height, margin, padding';
  item.style.transitionDuration = 500 + 'ms';
  item.style.height = height + 'px';
  item.style.removeProperty('padding-top');
  item.style.removeProperty('padding-bottom');
  item.style.removeProperty('margin-top');
  item.style.removeProperty('margin-bottom');
  window.setTimeout(() => {
    item.style.removeProperty('height');
    item.style.removeProperty('overflow');
    item.style.removeProperty('transition-duration');
    item.style.removeProperty('transition-property');
  }, 500);
}

function getMDFile() {
  const target = computeTargetPath() + localSetup.page + '.md';
  console.log('Get MD file: ' + target);
  fetch(target)
    .then(response => response.text())
    .then(content => populateViewDiv(content))
    .catch(error => {
      console.log('Error: ' + error);
      const mainPage = 'index';
      // get the main page instead
      if (localSetup.page !== mainPage) {
        localSetup.page = mainPage;
        getMDFile();
      }
    });
}

function getMenuFile() {
  const target = computeTargetPath() + 'menu.md';
  console.log('Get menu file: ' + target);
  fetch(target)
    .then(response => response.text())
    .then(content => receiveMenuContent(content))
    .catch(error => console.log('Error: ' + error));
}

function extractAnchor(url) {
  const match = /#([\w-]+)/.exec(url);
  if (match && match.length === 2)
    return match[1];
  return '';
}

// width: in pixels
function setHandleWidth(width) {
  handle.left.style.width = width + 'px';
  handle.menu.style.width = width + 'px';
  handle.handle.style.left = width + 'px';
  handle.center.style.left = width + 'px';
  handle.center.style.width = 'calc(100% - ' + width + 'px)';
}

function initializeHandle() {
  // inspired from: http://stackoverflow.com/questions/17855401/how-do-i-make-a-div-width-draggable
  handle = {}; // structure where all the handle info is stored

  handle.left = document.getElementById('left');
  handle.menu = document.getElementById('menu');
  handle.center = document.getElementById('center');
  handle.handle = document.getElementById('handle');
  handle.container = document.getElementById('webots-doc');

  // dimension bounds of the handle in pixels
  handle.min = 0;
  handle.minThreshold = 90; // under this threshold, the handle is totally hidden
  handle.initialWidth = Math.max(handle.minThreshold, parseFloat(getComputedStyle(handle.left, null).width.replace('px', '')));
  handle.max = Math.max(250, handle.initialWidth);

  handle.isResizing = false;
  handle.lastDownX = 0;

  if (isCyberboticsUrl) {
    handle.left.classList.add('cyberbotics');
    handle.handle.classList.add('cyberbotics');
    handle.center.classList.add('cyberbotics');
  } else {
    handle.left.classList.add('default');
    handle.handle.classList.add('default');
    handle.center.classList.add('default');
  }

  setHandleWidth(handle.initialWidth);

  handle.handle.onmousedown = _ => onSelectHandle(_);
  handle.handle.ontouchstart = _ => onSelectHandle(_);
  handle.handle.ondblclick = () => toggleHandle();

  document.onmousemove = _ => resizeHandle(_);
  document.ontouchmove = _ => resizeHandle(_);
  document.onmouseup = () => releaseHandle();
  document.ontouchend = () => releaseHandle();
}

function onSelectHandle(e) {
  if (e.type === 'touchstart')
    e = e.originalEvent.touches[0];
  handle.isResizing = true;
  handle.lastDownX = e.clientX;
  handle.container.style.userSelect = 'none';
}

function toggleHandle() {
  if (handle.left.style.width.startsWith('0'))
    setHandleWidth(handle.initialWidth);
  else
    setHandleWidth(0);
}

function resizeHandle(e) {
  if (e.type === 'touchmove')
    e = e.originalEvent.touches[0];
  if (!handle.isResizing)
    return;
  const mousePosition = e.clientX - (handle.container.getBoundingClientRect().left + document.body.scrollLeft); // in pixels
  if (mousePosition < handle.minThreshold / 2) {
    setHandleWidth(0);
    return;
  } else if (mousePosition < handle.minThreshold)
    return;
  if (mousePosition < handle.min || mousePosition > handle.max)
    return;
  setHandleWidth(mousePosition);
}

function releaseHandle() {
  handle.isResizing = false;
  handle.container.style.userSelect = 'auto';
}

window.onscroll = function() {
  if (!isCyberboticsUrl)
    return;
  updateMenuScrollbar();
};

window.mermaidAPI.initialize({startOnLoad: false});
initializeHandle();

if (!isCyberboticsUrl) {
  if (!localSetup.url)
    localSetup.url = getGETQueryValue('url', 'https://raw.githubusercontent.com/cyberbotics/webots/');
  if (!localSetup.book)
    localSetup.book = getGETQueryValue('book', 'guide');
  if (!localSetup.page)
    localSetup.page = getGETQueryValue('page', 'index');
  if (!localSetup.anchor)
    localSetup.anchor = window.location.hash.substring(1);
  if (!localSetup.branch)
    localSetup.branch = getGETQueryValue('branch', 'released');
  if (!localSetup.tabs)
    localSetup.tabs = getGETQueriesMatchingRegularExpression('^tab-\\w+$', 'g');
  // backward compatibility <= R2019b revision 1
  if (!localSetup.tabs['tab-language']) {
    if (localSetup.tab) {
      localSetup.tabs['tab-language'] = localSetup.tab;
      delete localSetup.tab;
    } else
      localSetup.tabs['tab-language'] = getGETQueryValue('tab', '').toLowerCase();
  }
}

// prevent FOUC for blog
if (localSetup.book === 'blog') {
  let center = document.querySelector('#center');
  center.setAttribute('class', 'blog');
  setHandleWidth(0);
}

addContributionBanner();
setupModalWindow();
applyToTitleDiv();
getMDFile();
getMenuFile();
