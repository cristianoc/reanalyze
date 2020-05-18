const fs = require("fs");
const child_process = require("child_process");
const path = require("path");

const spawn = child_process.spawn;

const isWindows = /^win/i.test(process.platform);

const reanalyzeNativePath = path.join(
  __dirname,
  "../../_esy/default/build/install/default/bin/reanalyze.exe"
);

const shell = isWindows ? true : false;

child_process.spawnSync(
  reanalyzeNativePath,
  ["-exception", "-ci", "-suppress", "src", "-unsuppress", "src/exception"],
  {
    stdio: ["inherit", "inherit"],
    shell,
  }
);
