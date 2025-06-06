// Created by qiuwenchen on 2023/6/9.
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

package com.tencent.wcdbtest.winq;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import static com.tencent.wcdbtest.base.WinqTool.winqEqual;

import com.tencent.wcdb.winq.RaiseFunction;
import com.tencent.wcdb.winq.StatementSelect;

import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
public class RaiseFunctionTest {
    @Test
    public void test() {
        winqEqual(new RaiseFunction().ignore(), "RAISE(IGNORE)");
        winqEqual(new RaiseFunction().rollback("testErrorMessage"),
                "RAISE(ROLLBACK, testErrorMessage)");
        winqEqual(new RaiseFunction().abort("testErrorMessage"),
                "RAISE(ABORT, testErrorMessage)");
        winqEqual(new RaiseFunction().fail("testErrorMessage"),
                "RAISE(FAIL, testErrorMessage)");
        winqEqual(new StatementSelect().select(new RaiseFunction().ignore()),
                "SELECT RAISE(IGNORE)");
    }
}
