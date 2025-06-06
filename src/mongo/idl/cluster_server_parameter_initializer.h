/**
 *    Copyright (C) 2022-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include "mongo/db/db_raii.h"
#include "mongo/db/operation_context.h"
#include "mongo/db/repl/replica_set_aware_service.h"
#include "mongo/db/service_context.h"

#include <string>

namespace mongo {

/**
 * An interface that provides methods to manipulate in-memory cluster server parameter values in
 * response to on-disk changes, specifically in a replica set context.
 */
class ClusterServerParameterInitializer
    : public ReplicaSetAwareService<ClusterServerParameterInitializer> {
    ClusterServerParameterInitializer(const ClusterServerParameterInitializer&) = delete;
    ClusterServerParameterInitializer& operator=(const ClusterServerParameterInitializer&) = delete;

public:
    ClusterServerParameterInitializer() = default;
    ~ClusterServerParameterInitializer() override = default;

    static ClusterServerParameterInitializer* get(OperationContext* opCtx);
    static ClusterServerParameterInitializer* get(ServiceContext* serviceContext);

    // Virtual methods coming from the ReplicaSetAwareService
    void onStartup(OperationContext* opCtx) final {}

    void onSetCurrentConfig(OperationContext* opCtx) final {}

    static void synchronizeAllParametersFromDisk(OperationContext* opCtx);
    /**
     * Called after startup recovery or initial sync is complete.
     */
    void onConsistentDataAvailable(OperationContext* opCtx, bool isMajority, bool isRollback) final;
    void onShutdown() final {}
    void onStepUpBegin(OperationContext* opCtx, long long term) final {}
    void onStepUpComplete(OperationContext* opCtx, long long term) final {}
    void onStepDown() final {}
    void onRollbackBegin() final {}
    void onBecomeArbiter() final {}
    inline std::string getServiceName() const final {
        return "ClusterServerParameterInitializer";
    }
};

}  // namespace mongo
