/* Adapted from https://github.com/LexiFi/dead_code_analyzer */

open DeadCommon;

let addTypeReference = (~posFrom, ~posTo) => {
  if (Common.debug^) {
    Log_.item(
      "addTypeReference %s --> %s@.",
      posFrom |> posToString,
      posTo |> posToString,
    );
  };
  TypeReferences.add(posTo, posFrom);
};

module TypeDependencies = {
  let delayedItems = ref([]);

  let add = (loc1, loc2) =>
    delayedItems := [(loc1, loc2), ...delayedItems^];

  let clear = () => delayedItems := [];

  let processTypeDependency =
      (
        (
          {loc_start: posTo, loc_ghost: ghost1}: Location.t,
          {loc_start: posFrom, loc_ghost: ghost2}: Location.t,
        ),
      ) =>
    if (!ghost1 && !ghost2 && posTo != posFrom) {
      addTypeReference(~posTo, ~posFrom);
    };

  let forceDelayedItems = () =>
    List.iter(processTypeDependency, delayedItems^);
};

let extendTypeDependencies = (loc1: Location.t, loc2: Location.t) =>
  if (loc1.loc_start != loc2.loc_start) {
    if (Common.debug^) {
      Log_.item(
        "extendTypeDependencies %s --> %s@.",
        loc1.loc_start |> posToString,
        loc2.loc_start |> posToString,
      );
    };
    TypeDependencies.add(loc1, loc2);
  };

// Type dependencies between Foo.re and Foo.rei
let addTypeDependenciesAcrossFiles = (~pathToType, ~loc, ~typeLabelName) => {
  let isInterface = Filename.check_suffix(Common.currentSrc^, "i");
  if (!isInterface) {
    let path_1 = pathToType |> Path.moduleToInterface;
    let path_2 = path_1 |> Path.typeToInterface;
    let path1 = [typeLabelName, ...path_1];
    let path2 = [typeLabelName, ...path_2];

    switch (TypeLabels.find(path1)) {
    | None =>
      switch (TypeLabels.find(path2)) {
      | None => ()
      | Some(loc2) =>
        extendTypeDependencies(loc, loc2);
        if (!Config.reportTypesDeadOnlyInInterface) {
          extendTypeDependencies(loc2, loc);
        };
      }
    | Some(loc1) =>
      extendTypeDependencies(loc, loc1);
      if (!Config.reportTypesDeadOnlyInInterface) {
        extendTypeDependencies(loc1, loc);
      };
    };
  } else {
    let path_1 = pathToType |> Path.moduleToImplementation;
    let path1 = [typeLabelName, ...path_1];
    switch (TypeLabels.find(path1)) {
    | None => ()
    | Some(loc1) =>
      extendTypeDependencies(loc1, loc);
      if (!Config.reportTypesDeadOnlyInInterface) {
        extendTypeDependencies(loc, loc1);
      };
    };
  };
};

// Add type dependencies between implementation and interface in inner module
let addTypeDependenciesInnerModule = (~pathToType, ~loc, ~typeLabelName) => {
  let path = [typeLabelName, ...pathToType];

  switch (TypeLabels.find(path)) {
  | Some(loc2) =>
    extendTypeDependencies(loc, loc2);
    if (!Config.reportTypesDeadOnlyInInterface) {
      extendTypeDependencies(loc2, loc);
    };
  | None => TypeLabels.add(path, loc)
  };
};

let addDeclaration = (~typeId: Ident.t, ~typeKind: Types.type_kind) => {
  let pathToType = [
    typeId |> Ident.name |> Name.create,
    ...Current.modulePath^ @ [Common.currentModuleName^],
  ];

  let processTypeLabel = (typeLabelName, ~declKind, ~loc: Location.t) => {
    addTypeDeclaration(~declKind, ~path=pathToType, ~loc, typeLabelName);

    addTypeDependenciesAcrossFiles(~pathToType, ~loc, ~typeLabelName);
    addTypeDependenciesInnerModule(~pathToType, ~loc, ~typeLabelName);

    TypeLabels.add([typeLabelName, ...pathToType], loc);
  };

  switch (typeKind) {
  | Type_record(l, _) =>
    List.iter(
      ({Types.ld_id, ld_loc}) => {
        Ident.name(ld_id)
        |> Name.create
        |> processTypeLabel(~declKind=RecordLabel, ~loc=ld_loc)
      },
      l,
    )
  | Type_variant(l) =>
    List.iter(
      ({Types.cd_id, cd_loc}) => {
        Ident.name(cd_id)
        |> Name.create
        |> processTypeLabel(~declKind=VariantCase, ~loc=cd_loc)
      },
      l,
    )
  | _ => ()
  };
};