#pragma once
// Testing purposes
#include <chime/futures/combine/par/all.hpp>
#include <chime/futures/combine/par/first.hpp>
#include <chime/futures/combine/seq/and_then.hpp>
#include <chime/futures/combine/seq/box.hpp>
#include <chime/futures/combine/seq/flat_map.hpp>
#include <chime/futures/combine/seq/flatten.hpp>
#include <chime/futures/combine/seq/map.hpp>
#include <chime/futures/combine/seq/or_else.hpp>
#include <chime/futures/combine/seq/start.hpp>
#include <chime/futures/combine/seq/via.hpp>
#include <chime/futures/make/contract.hpp>
#include <chime/futures/make/failure.hpp>
#include <chime/futures/make/just.hpp>
#include <chime/futures/make/submit.hpp>
#include <chime/futures/make/value.hpp>
#include <chime/futures/run/detach.hpp>
#include <chime/futures/run/get.hpp>
#include <chime/futures/syntax/bang.hpp>
#include <chime/futures/syntax/both.hpp>
#include <chime/futures/syntax/or.hpp>
#include <chime/futures/syntax/pipe.hpp>
using namespace futures;