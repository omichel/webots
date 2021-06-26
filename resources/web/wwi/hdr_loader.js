export default function loadHdr(url, onLoad) {
  const xmlhttp = new XMLHttpRequest();
  xmlhttp.open('GET', url, true);
  xmlhttp.responseType = 'arraybuffer';
  xmlhttp.overrideMimeType('arrayBuffer');
  xmlhttp.onreadystatechange = () => {
    if (xmlhttp.readyState === 4 && (xmlhttp.status === 200 || xmlhttp.status === 0)) { // Some browsers return HTTP Status 0 when using non-http protocol (for file://)
      onLoad(parse(xmlhttp.response));
    }
  };
  xmlhttp.send();
}

function parse(buffer) {
  let dec = new TextDecoder('utf-8');
  let dat = dec.decode(buffer);
  const lines = dat.split('\n');
  let number = lines[3] === '' ? 4 : 3;
  const dimension = lines[number].split(' ');
  let width = parseInt(dimension[1]);
  let height = parseInt(dimension[3]);

  const byteArray = new Uint8Array(buffer);
  byteArray.pos = lines[0].length + lines[1].length + lines[2].length + lines[3].length + 4;

  if (number === 4)
    byteArray.pos += lines[4].length + 1;

  const imageRgbaData = new Uint8Array(byteArray.subarray(byteArray.pos));
  let data;
  // adapted from http://www.graphics.cornell.edu/~bjw/rgbe.html
  const RGBEByteToRGBFloat = function(sourceArray, sourceOffset, destArray, destOffset) {
    const e = sourceArray[sourceOffset + 3];
    const scale = Math.pow(2.0, e - 128.0) / 255.0;
    destArray[destOffset + 0] = sourceArray[sourceOffset + 0] * scale;
    destArray[destOffset + 1] = sourceArray[sourceOffset + 1] * scale;
    destArray[destOffset + 2] = sourceArray[sourceOffset + 2] * scale;
  };
  const numElements = (imageRgbaData.length / 4) * 3;
  const floatArray = new Float32Array(numElements);
  for (let j = 0; j < numElements; j++)
    RGBEByteToRGBFloat(imageRgbaData, j * 4, floatArray, j * 3);

  data = floatArray;

  return {
    width: width,
    height: height,
    data: data
  };
}
