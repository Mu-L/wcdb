//
// Created by sanhuazhang on 2019/05/02
//

/*
 * Tencent is pleased to support the open source community by making
 * WCDB available.
 *
 * Copyright (C) 2017 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <WCDB/WINQ.h>

namespace WCDB {

WindowFunctionInvocation::~WindowFunctionInvocation() = default;

WindowFunctionInvocation::WindowFunctionInvocation(const UnsafeStringView& name)
{
    syntax().name = name;
}

WindowFunctionInvocation& WindowFunctionInvocation::arguments(const Expressions& expressions)
{
    syntax().expressions = expressions;
    syntax().wildcard = false;
    return *this;
}

WindowFunctionInvocation& WindowFunctionInvocation::argument(const Expression& expression)
{
    syntax().expressions.push_back(expression);
    syntax().wildcard = false;
    return *this;
}

WindowFunctionInvocation& WindowFunctionInvocation::invoke()
{
    syntax().expressions.clear();
    syntax().wildcard = false;
    return *this;
}

WindowFunctionInvocation& WindowFunctionInvocation::invokeAll()
{
    syntax().wildcard = true;
    return *this;
}

WindowFunctionInvocation& WindowFunctionInvocation::filter(const Filter& filter)
{
    syntax().filter = filter;
    return *this;
}

WindowFunctionInvocation& WindowFunctionInvocation::over(const WindowDef& windowDef)
{
    syntax().window.clear();
    syntax().windowDef = windowDef;
    return *this;
}

WindowFunctionInvocation& WindowFunctionInvocation::over(const UnsafeStringView& window)
{
    syntax().window = window;
    return *this;
}

} // namespace WCDB