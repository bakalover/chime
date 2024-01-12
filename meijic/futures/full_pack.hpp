#pragma once
#include <meijic/futures/combine/par/all.hpp>
#include <meijic/futures/combine/par/first.hpp>
#include <meijic/futures/combine/seq/and_then.hpp>
#include <meijic/futures/combine/seq/box.hpp>
#include <meijic/futures/combine/seq/flat_map.hpp>
#include <meijic/futures/combine/seq/flatten.hpp>
#include <meijic/futures/combine/seq/map.hpp>
#include <meijic/futures/combine/seq/or_else.hpp>
#include <meijic/futures/combine/seq/start.hpp>
#include <meijic/futures/combine/seq/via.hpp>
#include <meijic/futures/make/contract.hpp>
#include <meijic/futures/make/failure.hpp>
#include <meijic/futures/make/just.hpp>
#include <meijic/futures/make/submit.hpp>
#include <meijic/futures/make/value.hpp>
#include <meijic/futures/run/detach.hpp>
#include <meijic/futures/run/get.hpp>
#include <meijic/futures/syntax/bang.hpp>
#include <meijic/futures/syntax/both.hpp>
#include <meijic/futures/syntax/or.hpp>
#include <meijic/futures/syntax/pipe.hpp>
using namespace futures;