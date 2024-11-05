#pragma once

#include "bulk_updater.h"
#include "date.h"
#include "summing_segment_tree.h"

class BudgetManager {
   public:
    static const Date START_DATE;
    static const Date END_DATE;

    static size_t GetDayIndex(const Date& day) {
        return static_cast<size_t>(Date::ComputeDistance(START_DATE, day));
    }

    static IndexSegment MakeDateSegment(const Date& from, const Date& to) {
        return {GetDayIndex(from), GetDayIndex(to) + 1};
    }

    double ComputeSum(const Date& from, const Date& to) const {
        IncomeExpense day_info = tree_.ComputeSum(MakeDateSegment(from, to));
        return day_info.earned - day_info.spent;
    }

    void AddBulkOperation(const Date& from, const Date& to,
                          const BulkLinearUpdater& operation) {
        tree_.AddBulkOperation(MakeDateSegment(from, to), operation);
    }

   private:
    SummingSegmentTree<IncomeExpense, BulkLinearUpdater> tree_{
        GetDayIndex(END_DATE)};
};