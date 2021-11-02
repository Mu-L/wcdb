//
// Created by sanhuazhang on 2019/06/06
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

#include <WCDB/Assertion.hpp>
#include <WCDB/CoreConst.h>
#include <WCDB/Handle.hpp>
#include <WCDB/Notifier.hpp>
#include <WCDB/OperationHandle.hpp>
#include <WCDB/RepairKit.h>

namespace WCDB {

OperationHandle::OperationHandle()
: m_statementForIntegrityCheck(
StatementPragma().pragma(Pragma::integrityCheck()).with(1).schema(Schema::main()))
, m_statementForReadTransaction(StatementBegin().beginDeferred())
, m_statementForAcquireReadLock(
  StatementSelect().select(1).from(Syntax::masterTable).limit(0))
{
}

OperationHandle::~OperationHandle() = default;

void OperationHandle::doSuspend(bool suspend)
{
    AbstractHandle::doSuspend(suspend);
    BackupExclusiveDelegate::suspendBackup(suspend);
}

#pragma mark - Checkpoint
bool OperationHandle::checkpoint()
{
    return Handle::checkpoint(CheckpointMode::Passive);
}

#pragma mark - Integrity
void OperationHandle::checkIntegrity()
{
    auto optionalIntegrityMessages = getValues(m_statementForIntegrityCheck, 0);
    if (optionalIntegrityMessages.has_value()) {
        auto &integrityMessages = optionalIntegrityMessages.value();
        WCTAssert(integrityMessages.size() == 1);
        if (integrityMessages.size() > 0) {
            auto integrityMessage = *integrityMessages.begin();
            if (!integrityMessage.caseInsensiveEqual("ok")) {
                Error error(Error::Code::Corrupt, Error::Level::Error, integrityMessage);
                error.infos.insert_or_assign(ErrorStringKeyPath, getPath());
                error.infos.insert_or_assign(ErrorStringKeyType, ErrorTypeIntegrity);
                Notifier::shared().notify(error);
            }
        }
    }
}

#pragma mark - Backup
void OperationHandle::setBackupPath(const UnsafeStringView &path)
{
    Handle::setPath(path);
}

const StringView &OperationHandle::getBackupPath() const
{
    return Handle::getPath();
}

const Error &OperationHandle::getBackupError() const
{
    return Handle::getError();
}

bool OperationHandle::acquireBackupSharedLock()
{
    return execute(m_statementForReadTransaction) && execute(m_statementForAcquireReadLock);
}

bool OperationHandle::releaseBackupSharedLock()
{
    rollbackTransaction();
    return true;
}

bool OperationHandle::acquireBackupExclusiveLock()
{
    return beginTransaction();
}

bool OperationHandle::releaseBackupExclusiveLock()
{
    rollbackTransaction();
    return true;
}

void OperationHandle::finishBackup()
{
    Handle::close();
}

} // namespace WCDB