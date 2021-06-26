export let GtaoLevel = 4; // [0 - 4], 0 disable it
export const disableAntiAliasing = false;
export let disableShadows = false;
export const textureQuality = 2; // [0 - 2]
export const textureFiltering = 5; // [0 - 5]

function changeShadows() {
  disableShadows = !disableShadows;
  _wr_config_enable_shadows(!disableShadows);
}

function changeGtaoLevel(level) {
  GtaoLevel = level;
}

export {changeShadows, changeGtaoLevel};
