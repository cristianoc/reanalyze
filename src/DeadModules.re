let table = Hashtbl.create(1);

let markDead = (~loc, moduleName) => {
  switch (Hashtbl.find_opt(table, moduleName)) {
  | Some((false, _)) => ()
  | _ => Hashtbl.replace(table, moduleName, (false, loc))
  };
};

let markLive = (~loc: Location.t, moduleName) => {
  switch (Hashtbl.find_opt(table, moduleName)) {
  | None => Hashtbl.replace(table, moduleName, (true, loc))
  | Some(_) =>
    // Do nothing: if dead it stays dead, if live it stays live
    ()
  };
};

let checkModuleDead = (~fileName as pos_fname, moduleName) => {
  switch (Hashtbl.find_opt(table, moduleName)) {
  | Some((false, loc)) =>
    Hashtbl.remove(table, moduleName); // only report once
    let loc =
      if (loc.loc_ghost) {
        let pos = {Lexing.pos_fname, pos_lnum: 0, pos_bol: 0, pos_cnum: 0};
        {Location.loc_start: pos, loc_end: pos, loc_ghost: false};
      } else {
        loc;
      };

    Log_.info(~loc, ~name="Warning Dead Module", (ppf, ()) =>
      Format.fprintf(
        ppf,
        "@{<info>%s@} %s",
        moduleName,
        "is a dead module as all its items are dead.",
      )
    );

  | _ => ()
  };
};