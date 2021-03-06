/* TypeScript file generated from ImportHooks.res by genType. */
/* eslint-disable import/first */


import {makeRenamed as makeRenamedNotChecked} from './hookExample';

import {foo as fooNotChecked} from './hookExample';

// In case of type error, check the type of 'makeRenamed' in 'ImportHooks.re' and './hookExample'.
export const makeRenamedTypeChecked: (_1:unknown) => JSX.Element = makeRenamedNotChecked;

// Export 'makeRenamed' early to allow circular import from the '.bs.js' file.
export const makeRenamed: unknown = makeRenamedTypeChecked as (_1:unknown) => JSX.Element;

// In case of type error, check the type of 'foo' in 'ImportHooks.re' and './hookExample'.
export const fooTypeChecked: (_1:{ readonly person: person }) => string = fooNotChecked;

// Export 'foo' early to allow circular import from the '.bs.js' file.
export const foo: unknown = function (Argperson: any) {
  const result = fooTypeChecked({person:Argperson});
  return result
} as (_1:{ readonly person: person }) => string;

// tslint:disable-next-line:interface-over-type-literal
export type person = { readonly name: string; readonly age: number };

// tslint:disable-next-line:interface-over-type-literal
export type renderMe<a> = (_1:unknown) => JSX.Element;
