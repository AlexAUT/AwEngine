#pragma once

// Source: https://www.bfilipek.com/2018/06/variant.html

template <class... Ts>
struct Visitor : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
Visitor(Ts...)->Visitor<Ts...>;
